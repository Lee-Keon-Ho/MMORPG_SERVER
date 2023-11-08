#include <WS2tcpip.h>
#include "UserManager.h"
#include "LoginConnector.h"
#include "struct.h"
#include <iostream>

CLoginConnector::CLoginConnector(PCSTR _ip, u_short _port) : m_pRingBuffer(nullptr)
{
	SOCKADDR_IN servAddr;

	try
	{
		m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		memset(&servAddr, 0, sizeof(servAddr));
		servAddr.sin_family = AF_INET;
		inet_pton(AF_INET, _ip, &servAddr.sin_addr);
		servAddr.sin_port = htons(_port);

		connect(m_socket, (SOCKADDR*)&servAddr, sizeof(servAddr));
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	if (m_pRingBuffer == nullptr) m_pRingBuffer = new CRingBuffer();
	if (m_pRingBuffer == nullptr)
	{
		printf("Create RingBuffer Error\n");
	}
}

CLoginConnector::~CLoginConnector()
{
	if (m_pRingBuffer) { delete m_pRingBuffer; m_pRingBuffer = nullptr; }
}

bool CLoginConnector::Start()
{
	if (!CThread::Start()) return false;
	return true;
}

void CLoginConnector::RunLoop()
{
	int recvSize = 0;
	char buffer[100];
	while (true)
	{
		/*recvSize = recv(m_socket, m_pRingBuffer->GetWriteBuffer(), m_pRingBuffer->GetWriteBufferSize(), 0);*/
		recvSize = recvSize = recv(m_socket, buffer, 100, 0);
		m_pRingBuffer->Write(recvSize);

		// 정해진 packet만 온다.
		// 누가 로그인 완료 되었다.
		// key Values

		// test
		/*char* tempBuffer = m_pRingBuffer->GetReadBuffer();*/
		char* tempBuffer = buffer;
		int size = *(u_short*)tempBuffer;
		tempBuffer += sizeof(u_short);
		int type = *(u_short*)tempBuffer;
		tempBuffer += sizeof(u_short);
		int len = *(u_short*)tempBuffer;
		tempBuffer += sizeof(u_short);

		sCharacterInfo characterInfo;

		memcpy(&characterInfo, tempBuffer, sizeof(sCharacterInfo));

		/*wchar_t name[28] = { 0, };
		memcpy(name, tempBuffer, len);
		tempBuffer += len;
		int characterType = *(int*)tempBuffer;
		tempBuffer += sizeof(int);
		int level = *(int*)tempBuffer;
		tempBuffer += sizeof(int);
		int curHp = *(int*)tempBuffer;
		tempBuffer += sizeof(int);
		int maxHp = *(int*)tempBuffer;
		tempBuffer += sizeof(int);
		int curMp = *(int*)tempBuffer;
		tempBuffer += sizeof(int);
		int maxMp = *(int*)tempBuffer;
		tempBuffer += sizeof(int);
		float posX = *(float*)tempBuffer;
		tempBuffer += sizeof(float);
		float posY = *(float*)tempBuffer;
		tempBuffer += sizeof(float);
		float posZ = *(float*)tempBuffer;
		tempBuffer += sizeof(float);
		int curExp = *(int*)tempBuffer;
		tempBuffer += sizeof(int);
		int damageMin = *(int*)tempBuffer;
		tempBuffer += sizeof(int);
		int damageMax = *(int*)tempBuffer;
		tempBuffer += sizeof(int);*/

		m_pRingBuffer->Read(recvSize);

		CUserManager::GetInstance()->Add(characterInfo,new CUser(characterInfo));
	}
}
