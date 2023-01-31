#include "Acceptor.h"
#include <stdio.h>

CAcceptor::CAcceptor()
{
}

CAcceptor::CAcceptor(SOCKET _socket) : m_socket(_socket)
{

}

CAcceptor::~CAcceptor()
{
}

bool CAcceptor::Start()
{
	if (!CThread::Start()) return false;;
	return true;
}

void CAcceptor::RunLoop()
{
	ACCEPT_SOCKET_INFO socketInfo;
	int size = sizeof(sockaddr_in);

	while (1)
	{
		socketInfo.socket = accept(m_socket, (sockaddr*)&socketInfo.addr, &size);

		printf("socket : %d \n", socketInfo.socket);

		// 이 부분을 virtual func로 만들고 상속 받아서 override해서 쓰는 방식
		Handle(socketInfo);
	}
}
