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
		Sleep(10000);
		
		int rand = Random(array_max_size - 1, 0);

		LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_NOTICE(chat_max_size, m_list[rand]);

		CUserManager::GetInstance()->SendAll(packet, sizeof(FS2C_PACKET_NOTICE));
	}
}
