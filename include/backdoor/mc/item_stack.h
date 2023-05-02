#pragma once
#include <littlehooker/littlehooker.h>
#include <backdoor/cpp_string.h>
#include "actor.h"

struct item_stack;

struct item_stack *get_carried_item(struct actor *actor);
