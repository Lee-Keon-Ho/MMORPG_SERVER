#pragma once
#include "User.h"
#include "Monster.h"
#include "Sector.h"
#include "sectorStruct.h"
#include "PacketType.h"
#include "MapGrid.h"
#include <vector>

class CMap // field // field에 sector가 있다 field에 유저가 들어가는 거다
{
public:
	typedef std::vector<CSector*> Sector_t;
	CMapGrid* m_pMapGrid;

private:
	Sector_t m_sector;
public:
	CMap();
	CMap(const char* _fileName);
	~CMap();

	void Add(CUser* _pUser, int _sector);
	void Del(CUser* _pUser, int _sector);
	void Add(CMonster* _pMonster, int _sector);
	void Del(CMonster* _pMonster, int _sector);

	void InSector(CUser& _user);
	void OutSector(CUser& _user);

	void SendAll(char* _buffer, int _size, int _sector);

	std::map<SOCKET, CUser*> GetMap(int _sector);

	int GetSectorCount(int _sector);
	CMapGrid* GetMapGrid();

	CSector* GetSector(int _index);
	std::vector<CSector*> SetDifference(int _a, int _b);
	void DifferenceSend(char* _buffer, int _size, int _a, int _b);
};