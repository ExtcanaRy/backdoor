#pragma once
#include <littlehooker/littlehooker.h>
#include "mc/level.h"
#include "mc/player.h"

char** slipt_cmd(const char* str, int* count);
void free_slipt_cmd(char **array, int count);
void process_remote_cmd(const char *cmd);
