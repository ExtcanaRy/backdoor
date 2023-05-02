#pragma once
#include <littlehooker/littlehooker.h>
#include <backdoor/cpp_string.h>
#include <backdoor/plugin.h>
#include "actor.h"
#include "spsc_queue.h"

struct level;

struct player *get_player_by_xuid(struct level *level, const char *xuid);
struct player *get_player_by_name(struct level *level, const char *name);
void run_game_cmd(const char *cmd);

extern struct level *g_level;
