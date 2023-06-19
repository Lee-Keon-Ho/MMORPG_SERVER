#include "Map.h"
#include "UserManager.h"
CMap::CMap()
{
	for (int i = 0; i < 43; i++)
	{
		for (int x = 0; x < 43; x++)
		{
			m_sector.push_back(new CSector(x, i));
		}
	}

	int dx[] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };
	int dy[] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };

	int nx;
	int ny;

	for (int y = 0; y < 43; y++) {
		for (int x = 0; x < 43; x++) {
			int index = y * 43 + x;
			
			for (int i = 0; i < 9; i++) {
				int nx = x + dx[i];
				int ny = y + dy[i];
				if (nx < 0 || nx >= 43 || ny < 0 || ny >= 43) continue;

				m_sector[index]->SetAdjacentSector(m_sector[ny * 43 + nx]);
			}
		}
	}
}

CMap::~CMap()
{
	
}

void CMap::Add(CUser* _pUser, int _sector)
{
	m_sector[_sector]->Add(_pUser);
}

void CMap::Del(CUser* _pUser, int _sector)
{
	m_sector[_sector]->Del(_pUser);
}

void CMap::CheckSectorUpdates(CUser* _pUser)
{
	if (!_pUser->checkSector())
	{
		Del(_pUser, _pUser->GetPrevSector());
		Add(_pUser, _pUser->GetNowSector());
		OutSector(*_pUser);
		InSector(*_pUser);
		_pUser->SetPrevSector();
		_pUser->SetSector();

		//CUserManager::GetInstance()->SendUserCount();
	}
}

void CMap::InSector(CUser& _user)
{
	PACKET_INSECTOR packet;

	packet.size = sizeof(PACKET_INSECTOR);
	packet.type = CS_PT_INSECTOR;
	packet.number = _user.GetNumber();
	packet.currentPosition = *_user.GetPosition();
	packet.endPosition = *_user.GetEndPosition();

	int prevSector = _user.GetPrevSector(); // a
	int nowSector = _user.GetNowSector(); // b

	// b - a
	std::vector<CSector*> result = SetDifference(nowSector, prevSector);

	std::vector<CSector*>::iterator iter = result.begin();
	std::vector<CSector*>::iterator iterEnd = result.end();

	CSector* pSector;

	for (; iter != iterEnd; iter++)
	{
		pSector = *iter;
		pSector->Send(reinterpret_cast<char*>(&packet), sizeof(PACKET_INSECTOR));
		pSector->FetchUserInfoInNewSector(_user);
	}
}

void CMap::OutSector(CUser& _user)
{
	PACKET_OUTSECTOR packet;

	packet.size = static_cast<u_short>(sizeof(PACKET_OUTSECTOR));
	packet.type = static_cast<u_short>(CS_PT_OUTSECTOR);
	packet.number = static_cast<u_short>(_user.GetNumber());

	int prevSector = _user.GetPrevSector(); // a
	int nowSector = _user.GetNowSector(); // b

	// a - b
	std::vector<CSector*> result = SetDifference(prevSector, nowSector);

	std::vector<CSector*>::iterator iter = result.begin();
	std::vector<CSector*>::iterator iterEnd = result.end();

	CSector* pSector;

	for (; iter != iterEnd; iter++)
	{
		pSector = *iter;
		pSector->Send(reinterpret_cast<char*>(&packet), sizeof(PACKET_OUTSECTOR));
		pSector->DeleteUsersOutOfSector(_user);
	}
}

void CMap::SendAll(char* _buffer, int _size, int _sector)
{
	m_sector[_sector]->SendAll(_buffer, _size);
}

std::map<SOCKET, CUser*> CMap::GetMap(int _sector)
{
	return m_sector[_sector]->GetMap();
}

int CMap::GetSectorCount(int _sector)
{
	return m_sector[_sector]->Size();
}

CSector* CMap::GetSector(int _index)
{
	return m_sector[_index];
}

std::vector<CSector*> CMap::SetDifference(int _a, int _b)
{
	return m_sector[_a]->Difference(m_sector[_b]->GetAdjacentSector());
}