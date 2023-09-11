#include "Notice.h"
#include "UserManager.h"
#include "VECTOR.h"

CNotice::CNotice()
{
}

CNotice::~CNotice()
{
}


void CNotice::Start()
{
	CThread::Start();
}

void CNotice::RunLoop()
{
	while (true)
	{
		Sleep(1000);
		
		int rand = Random(MAX_ARRAY_SIZE - 1, 0);

		char sendBuffer[50];
		char* tempBuffer = sendBuffer;
		*(u_short*)tempBuffer = 6 + MAX_CHAR_SIZE;
		tempBuffer += sizeof(u_short);
		*(u_short*)tempBuffer = CS_PT_NOTICE;
		tempBuffer += sizeof(u_short);
		*(u_short*)tempBuffer = MAX_CHAR_SIZE;
		tempBuffer += sizeof(u_short);
		memcpy(tempBuffer, m_list[rand], MAX_CHAR_SIZE);
		tempBuffer += MAX_CHAR_SIZE;

		CUserManager::GetInstance()->SendAll(sendBuffer, tempBuffer - sendBuffer);
	}
}
