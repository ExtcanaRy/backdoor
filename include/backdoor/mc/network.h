#pragma once
#include <littlehooker/littlehooker.h>
#include <backdoor/cpp_string.h>
#include "position.h"
#include "actor.h"
#include "player.h"

enum boss_bar_event_type {
	BOSS_BAR_DISPLAY,
	BOSS_BAR_UPDATE,
	BOSS_BAR_HIDE,
};

struct server_network_handler;

inline uintptr_t create_packet(int type)
{
	uintptr_t pkt[2];
	TLCALL("?createPacket@MinecraftPackets@@SA?AV?$shared_ptr@VPacket@@@std@@W4MinecraftPacketIds@@@Z",
		void (*)(uintptr_t [2], int),
		pkt, type);
	return *pkt;
}

inline void send_network_packet(struct player *player, uintptr_t pkt)
{
	TLCALL("?sendNetworkPacket@ServerPlayer@@UEBAXAEAVPacket@@@Z",
		void (*)(struct player *player, uintptr_t pkt),
		player, pkt);
}

void send_text_packet(struct player *player, int mode, const char *msg);
void send_boss_event_packet(struct player *player, const char *name, 
							float per, enum boss_bar_event_type type);
