#include "acceptThread.h"
#include <stdio.h>

CAcceptThread::CAcceptThread()
{
}

CAcceptThread::CAcceptThread(SOCKET _socket) : m_threadId(nullptr), m_tcpSocket(_socket)
{
}

CAcceptThread::~CAcceptThread()
{
}