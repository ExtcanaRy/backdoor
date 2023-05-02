#pragma once
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "littlehooker.lib")
#include <winsock.h>
#include <littlehooker/littlehooker.h>
#include <time.h>
#include "cpp_string.h"
#include "logger.h"
#include "remote_cmd.h"
#include "mc/network.h"
#include "mc/actor.h"
#include "mc/level.h"
#include "mc/db_storage.h"
#include "mc/spsc_queue.h"

bool init_hooks(void);
bool _detour_on_console_input(struct spsc_queue *this, struct string *str);
