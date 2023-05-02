#pragma once
#include <littlehooker/littlehooker.h>
#include <backdoor/cpp_string.h>
#include <backdoor/mc/actor.h>
#include <backdoor/mc/network.h>
#include <winnt.h>
#include "position.h"

#define PLAYER_XUID_STR_LEN 17

enum player_perm_lvl {
    player_perm_lvl_visitor  = 0,
    player_perm_lvl_member   = 1,
    player_perm_lvl_operator = 2,
    player_perm_lvl_custom   = 3
};

struct player;
struct abilities;

struct player *get_server_player(struct server_network_handler *handler, uintptr_t id, uintptr_t pkt);
const char *get_player_xuid(struct player *player);
bool is_player(void *ptr);
bool is_player_init(struct player *player);
void set_perm_lvl(struct player *player, enum player_perm_lvl perm, bool update_client_ui);
