#include "Map.h"
#include "UserManager.h"
CMap::CMap() : m_pMapGrid(nullptr)
{
	for (int i = 0; i < sector_line; ++i)
	{
		for (int x = 0; x < sector_line; ++x)
		{
			m_sector.push_back(new CSector(x, i));
		}
	}

	int dx[] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };
	int dy[] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };

	int nx;
	int ny;

	for (int y = 0; y < sector_line; ++y)
	{
		for (int x = 0; x < sector_line; ++x)
		{
			int index = y * sector_line + x;

			for (int i = 0; i < 9; ++i)
			{
				nx = x + dx[i];
				ny = y + dy[i];
				if (nx < 0 || nx >= sector_line || ny < 0 || ny >= sector_line) continue;

				m_sector[index]->SetAdjacentSector(m_sector[ny * sector_line + nx]);
			}
		}
	}
}

CMap::CMap(const char* _fileName) : m_pMapGrid(nullptr)
{
	for (int i = 0; i < sector_line; ++i)
	{
		for (int x = 0; x < sector_line; ++x)
		{
			m_sector.push_back(new CSector(x, i));
		}
	}

	int dx[] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };
	int dy[] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };

	int nx;
	int ny;

	for (int y = 0; y < sector_line; ++y)
	{
		for (int x = 0; x < sector_line; ++x)
		{
			int index = y * sector_line + x;

			for (int i = 0; i < 9; ++i)
			{
				nx = x + dx[i];
				ny = y + dy[i];
				if (nx < 0 || nx >= sector_line || ny < 0 || ny >= sector_line) continue;

				m_sector[index]->SetAdjacentSector(m_sector[ny * sector_line + nx]);
			}
		}
	}
	if (m_pMapGrid == nullptr) m_pMapGrid = new CMapGrid(_fileName);
}

CMap::~CMap()
{
	if (m_pMapGrid != nullptr) { delete m_pMapGrid; m_pMapGrid = nullptr; }
}

void CMap::Add(CUser* _pUser, int _sector)
{
	m_sector[_sector]->Add(_pUser);
}

void CMap::Del(CUser* _pUser, int _sector)
{
	m_sector[_sector]->Del(_pUser);
}

void CMap::Add(CMonster* _pMonster, int _sector)
{
	m_sector[_sector]->Add(_pMonster);
}

void CMap::Del(CMonster* _pMonster, int _sector)
{
	m_sector[_sector]->Del(_pMonster);
}

void CMap::InSector(CUser& _user)
{
	LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_INSECTOR(_user.GetIndex(), *_user.GetPosition(), *_user.GetEndPosition());

	int prevSector = _user.GetPrevSector(); // a
	int nowSector = _user.GetNowSector(); // b

	// b - a
	std::vector<CSector*> result = SetDifference(nowSector, prevSector);

	for (CSector* pSector : result)
	{
		pSector->Send(packet, sizeof(FS2C_PACKET_INSECTOR));
		pSector->FetchUserInfoInNewSector(_user);
		pSector->FetchMonsterInfoInNewSector(_user);
	}
}

void CMap::OutSector(CUser& _user)
{
	LKH::sharedPtr<PACKET> packet = new FS2C_PACKET_OUTSECTOR(_user.GetIndex());

	int prevSector = _user.GetPrevSector(); // a
	int nowSector = _user.GetNowSector(); // b

	// a - b
	std::vector<CSector*> result = SetDifference(prevSector, nowSector);

	for (CSector* pSector : result)
	{
		pSector->Send(packet, sizeof(FS2C_PACKET_OUTSECTOR));
		pSector->DeleteUsersOutOfSector(_user);
		pSector->DeleteMonstersOutOfSector(_user);
	}
}

void CMap::SendAll(LKH::sharedPtr<PACKET> _buffer, int _size, int _sector)
{
	m_sector[_sector]->SendAll(_buffer, _size);
}

int CMap::GetSectorCount(int _sector)
{
	return m_sector[_sector]->Size();
}

CMapGrid* CMap::GetMapGrid()
{
	return m_pMapGrid;
}

CSector* CMap::GetSector(int _index)
{
	return m_sector[_index];
}

std::vector<CSector*> CMap::SetDifference(int _a, int _b)
{
	return m_sector[_a]->Difference(m_sector[_b]->GetAdjacentSector());
}

void CMap::DifferenceSend(LKH::sharedPtr<PACKET> _buffer, int _size, int _a, int _b)
{
	std::vector<CSector*> result = m_sector[_a]->Difference(m_sector[_b]->GetAdjacentSector());

	for (CSector* pSector : result)
	{
		pSector->Send(_buffer, _size);
	}
}
