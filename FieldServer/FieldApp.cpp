#include "FieldApp.h"
#include "../NetCore/IOCP.h"
#include "../NetCore/workerThread.h"
#include "UserManager.h"
#include "User.h"
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
	WSADATA wsaData; // NetCore¿¡¼­ Init
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

	if (m_pFieldServerAccept == nullptr) m_pFieldServerAccept = new CFieldServerAccept(m_pListener->GetSocket());
	if (!m_pFieldServerAccept) return false;

	//m_pFieldServerAccept = new CFieldServerAccept(new CTcpListener("183.108.148.83", 30002));

	return true;
}

bool CFieldApp::StartInstance()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	if (!m_pThreadManager->Start(si.dwNumberOfProcessors * 2)) return false;
	if (!m_pFieldServerAccept->Start()) return false;
	if (!m_pListener->Start()) return false;

	printf("server start...\n");
	return true;
}

void CFieldApp::RunLoop()
{
	CUser* pUser = nullptr;

	while (true)
	{
		m_user;// = CUserManager::GetInstatnce();

		std::vector<CUser*>::iterator iter = m_user.begin();
		std::vector<CUser*>::iterator iterEnd = m_user.end();

		for (; iter != iterEnd; iter++)
		{
			pUser = *iter;
			while (true)
			{
				if (pUser->GetReadSize() > 0) pUser->PacketHandle();
				else break;
			}
		}

		Sleep(1);
	}
}

void CFieldApp::DeleteInstance()
{
	if (m_pListener) { delete m_pListener; m_pListener = nullptr; }
}