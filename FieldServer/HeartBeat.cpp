#include "HeartBeat.h"
#include "UserManager.h"
CHeartBeat::CHeartBeat()
{
}

CHeartBeat::~CHeartBeat()
{
}

void CHeartBeat::Check()
{
	CUserManager* userManager = CUserManager::GetInstance();
	std::map<SOCKET, std::unique_ptr<CUser>> userList = userManager->GetUserList();

	LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_HEART_BEAT();

	for (auto& user : userList)
	{
		/*if (user.second->checkHeartBeat()) user.second->Send(packet, sizeof(FS2C_PACKET_HEART_BEAT));
		else
		{
			userManager->Del(user.first);
		}*/
	}

	userManager->retUser(std::move(userList));
}