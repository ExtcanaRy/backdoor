#include <backdoor/mc/level.h>

struct level *g_level = NULL;

struct player *get_player_by_xuid(struct level *level, const char *xuid)
{
	struct string *xuid_cpp_str = std_string_string(xuid);
	struct player *player =
		TLCALL("?getPlayerByXuid@Level@@UEBAPEAVPlayer@@AEBV?$basic_string@"
				"DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z",
			struct player *(*)(struct level *level, struct string *xuid),
			level, xuid_cpp_str);
	free(xuid_cpp_str);
	return player;
}

struct player *get_player_by_name(struct level *level, const char *name)
{
	struct string *name_cpp_str = std_string_string(name);
	struct player *player =
		TLCALL("?getPlayer@Level@@UEBAPEAVPlayer@@AEBV?$basic_string@"
				"DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z",
			struct player *(*)(struct level *level, struct string *name),
			level, name_cpp_str);
	free(name_cpp_str);
	return player;
}

void run_game_cmd(const char *cmd)
{
	if (g_spsc_queue) {
		struct string *list = std_string_string(cmd);
		_detour_on_console_input(g_spsc_queue, list);
		free(list);
	}
}
