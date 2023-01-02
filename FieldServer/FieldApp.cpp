#include "FieldApp.h"
#include "../NetCore/IOCP.h"
#include "../NetCore/workerThread.h"
#include "CUser.h"
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
	printf("server start...\n");
	return true;
}

void CFieldApp::RunLoop()
{
	while (true)
	{
		// accept Thread
		ACCEPT_SOCKET_INFO socketInfo;
		int size = sizeof(sockaddr_in);

		socketInfo.socket = accept(m_pListener->GetSocket(), (sockaddr*)&socketInfo.addr, &size);

		CUser* pUser = new CUser(socketInfo);
		CIocp::GetInstance()->Associate(socketInfo.socket);

		pUser->Recv();// OnConnect() 뭔가 설정할게 있다면 // player Map을 이용
	}
}

void CFieldApp::DeleteInstance()
{
	if (m_pListener) { delete m_pListener; m_pListener = nullptr; }
}

bool CFieldApp::ThreadStart()
{
	CWorkerThread workerThread;
	SYSTEM_INFO si;

	GetSystemInfo(&si);
	for (unsigned int i = 0; i < si.dwNumberOfProcessors * 2; ++i)
	{
		if (!workerThread.Start()) return false; // 예외
	}
	return true;
}
