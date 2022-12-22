#include "FieldApp.h"
#include "../NetCore/IOCP.h"
#include "../NetCore/workerThread.h"
#include <stdio.h>

#pragma comment (lib, "./../x64/Debug/NetCore.lib")
CFieldApp::CFieldApp() : m_pListener(nullptr)
{
}

CFieldApp::~CFieldApp()
{
}

bool CFieldApp::Initialize()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed WSAStartup()\n");
		return false;
	}

	return true;
}

bool CFieldApp::CreateInstance()
{
	if (m_pListener == nullptr) m_pListener = new CTcpListener("183.108.148.83", 30002);
	if (!m_pListener) return false;

	return true;
}

bool CFieldApp::StartInstance()
{
	if (!ThreadStart()) return false;;
	if (!m_pListener->Start()) return false;
	return true;
}

void CFieldApp::RunLoop()
{
	while (true)
	{
		ACCEPT_SOCKET_INFO socketInfo = Accept();

		//CPlayer* pPlayer = new CPlayer(socket);
		//CIocp::GetInstance()->Associate(socket, (CSession*)pPlayer);

		//pPlayer->WsaRecv(); // OnConnect() 뭔가 설정할게 있다면
	}
}

void CFieldApp::DeleteInstance()
{
	if (m_pListener) { delete m_pListener; m_pListener = nullptr; }
}

ACCEPT_SOCKET_INFO CFieldApp::Accept()
{
	int size = sizeof(sockaddr_in);

	ACCEPT_SOCKET_INFO clientInfo;

	clientInfo.socket = accept(m_pListener->GetSocket(), (sockaddr*)&clientInfo.addr, &size);

	// 예외 처리

	return clientInfo;
}

bool CFieldApp::ThreadStart()
{
	CWorkerThread workerThread;
	SYSTEM_INFO si;

	GetSystemInfo(&si);
	for (unsigned int i = 0; i < si.dwNumberOfProcessors * 2; ++i)
	{
		if (!workerThread.Start()) return false; // 문제가 생겼다
	}
	return true;
}
