#include "App.h"
#include "../NetCore/IOCP.h"
#include <stdio.h>

#pragma comment (lib, "./../x64/Debug/NetCore.lib")

CApp::CApp()
{
}

CApp::~CApp()
{
}

bool CApp::Initialize()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed WSAStartup()\n");
		return false;
	}

	return true;
}

void CApp::Run()
{
	while (true)
	{
		ACCEPT_SOCKET_INFO socketInfo = Accept();

		//CPlayer* pPlayer = new CPlayer(socket);
		//CIocp::GetInstance()->Associate(socket, (CSession*)pPlayer);

		//pPlayer->WsaRecv(); // OnConnect() 뭔가 설정할게 있다면
	}
}

void CApp::Destroy()
{
}

void CApp::CreateInstance()
{
	if (m_pListener == nullptr) m_pListener = new CTcpListener("183.108.148.83", 30002);
}

void CApp::StartInstance()
{
	//thread
	m_pListener->Start();
}

ACCEPT_SOCKET_INFO CApp::Accept()
{
	int size = sizeof(sockaddr_in);

	ACCEPT_SOCKET_INFO clientInfo;

	clientInfo.socket = accept(m_pListener->GetSocket(), (sockaddr*)&clientInfo.addr, &size);

	// 예외

	return clientInfo;
}
