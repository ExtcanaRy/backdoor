#include <backdoor/mc/network.h>

void send_text_packet(struct player *player, int mode, const char *msg)
{
	uintptr_t pkt = create_packet(9);
	const char *player_name = get_name_tag((struct actor *)player);
	struct string *player_name_cpp_str = std_string_string(player_name);
	struct string *msg_cpp_str = std_string_string(msg);
	DEREFERENCE(int, pkt, 48) = mode;
 	memcpy((void *)(pkt + 56), player_name_cpp_str, 32);
 	memcpy((void *)(pkt + 88), msg_cpp_str, 32);
	send_network_packet(player, pkt);
	free(player_name_cpp_str);
	free(msg_cpp_str);
}

void send_boss_event_packet(struct player *player, const char *name,
							float per, enum boss_bar_event_type type)
{
	uintptr_t pkt = create_packet(74);
	uintptr_t unique_id = DEREFERENCE(uintptr_t, get_unique_id((struct actor *)player), 0);
	struct string *name_cpp_str = std_string_string(name);
	DEREFERENCE(uintptr_t, pkt, 56) = unique_id;
	DEREFERENCE(int, pkt, 72) = type;
 	memcpy((void *)(pkt + 80), name_cpp_str, 32);
	DEREFERENCE(float, pkt, 112) = per;
	send_network_packet(player, pkt);
	free(name_cpp_str);
}
