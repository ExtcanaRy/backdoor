#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include <winsock.h>

extern SOCKET g_server_socket;
extern SOCKADDR *g_client_addr;
