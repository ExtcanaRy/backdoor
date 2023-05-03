#pragma once
#include <littlehooker/littlehooker.h>
#include <backdoor/cpp_string.h>
#include <backdoor/plugin.h>
#include "actor.h"
#include "spsc_queue.h"

struct level;

struct player *get_player_by_xuid(const char *xuid);
struct player *get_player_by_name(const char *name);
void run_game_cmd(const char *cmd);
const char* get_player_names(void);

extern struct level *g_level;
