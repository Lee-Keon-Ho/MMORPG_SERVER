#pragma once
#include "session.h"
#include <WinSock2.h>

struct overlapped_ex : WSAOVERLAPPED
{
	CSession* session;
};