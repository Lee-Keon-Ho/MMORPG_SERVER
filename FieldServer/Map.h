#pragma once
#include "User.h"
#include "Monster.h"
#include "Sector.h"
#include "sectorStruct.h"
#include "PacketType.h"
#include <vector>

class CMap
{
public:
	static CMap* GetInstance()
	{
		static CMap instance;
		return &instance;
	}

private:
	CMap();
	~CMap();
	
public:
	typedef std::vector<CSector*> Sector_t;
	
private:
	Sector_t m_sector;

	int m_userNumber;
public:
	void Add(CUser* _pUser, int _sector);
	void Del(CUser* _pUser, int _sector);
	void Add(CMonster* _pMonster, int _sector);
	void Del(CMonster* _pMonster, int _sector);

	void CheckSectorUpdates(CUser* _pUser);
	void InSector(CUser& _user);
	void OutSector(CUser& _user);

	void SendAll(char* _buffer, int _size, int _sector);

	std::map<SOCKET, CUser*> GetMap(int _sector);

	int GetSectorCount(int _sector);

	CSector* GetSector(int _index);
	std::vector<CSector*> SetDifference(int _a, int _b);
	void DifferenceSend(char* _buffer, int _size, int _a, int _b);
};