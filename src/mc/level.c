#include <backdoor/mc/level.h>

struct level *g_level = NULL;

struct player *get_player_by_xuid(const char *xuid)
{
	struct string *xuid_cpp_str = std_string_string(xuid);
	struct player *player =
		TLCALL("?getPlayerByXuid@Level@@UEBAPEAVPlayer@@AEBV?$basic_string@"
				"DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z",
			struct player *(*)(struct level *level, struct string *xuid),
			g_level, xuid_cpp_str);
	free(xuid_cpp_str);
	return player;
}

struct player *get_player_by_name(const char *name)
{
	struct string *name_cpp_str = std_string_string(name);
	struct player *player =
		TLCALL("?getPlayer@Level@@UEBAPEAVPlayer@@AEBV?$basic_string@"
				"DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z",
			struct player *(*)(struct level *level, struct string *name),
			g_level, name_cpp_str);
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

const char *get_player_names(void)
{
	struct string *cpp_str = calloc(4096, 1);
	TLCALL("?getPlayerNames@Level@@UEAA?AV?$basic_string@"
		"DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ",
		struct string *(*)(struct level *, struct string *),
		g_level, cpp_str);
	return std_string_c_str(cpp_str);
}
