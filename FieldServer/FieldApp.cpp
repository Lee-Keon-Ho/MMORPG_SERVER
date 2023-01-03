#include "FieldApp.h"
#include "../NetCore/IOCP.h"
#include "../NetCore/workerThread.h"
#include "CUser.h"
#include <stdio.h>

#pragma comment (lib, "./../x64/Debug/NetCore.lib")

CFieldApp::CFieldApp() : m_pListener(nullptr), m_pThreadManager(nullptr)
{
}

CFieldApp::~CFieldApp()
{
}

bool CFieldApp::Initialize()
{
	WSADATA wsaData; // NetCore에서 Init
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

	if (m_pThreadManager == nullptr) m_pThreadManager = new CThreadManager();
	if (!m_pThreadManager) return false;

	return true;
}

bool CFieldApp::StartInstance()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	if (!m_pThreadManager->Start(si.dwNumberOfProcessors * 2)) return false;
	if (!m_pListener->Start()) return false;
	
	printf("server start...\n");
	return true;
}

void CFieldApp::RunLoop()
{
	while (true) // 01-03 지금은 비어있는 걸로
	{
		// UserHandler 모아서 처리
		// 모든 user Vector
		while (true)
		{
			//if (user->GetReadSize() > 0) PakcetHandle();

			//if(user->GetReadSize() == 0 ) break;
		}
		

		// accept Thread 아직도 잘 모르겠다 명확해야한다.
		
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