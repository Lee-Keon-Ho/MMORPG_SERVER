#pragma once
#include "User.h"
#include "PacketStruct.h"
#include "PacketType.h"
#include "Monster.h"
#include <map>
#include <vector>

#define SECTOR_SIZE 18
#define SECTOR_LINE 15

class CSector
{
private:
	typedef std::map<SOCKET, CUser*> userMap_t;
	typedef std::vector<CSector*> sector_t;
	typedef std::map<int, CMonster*> monster_t;
private:
	userMap_t m_userList;
	monster_t m_monsterList;

	CRITICAL_SECTION m_cs_user;
	CRITICAL_SECTION m_cs_monster;

	sector_t m_AdjacentSector;

	int nSector; // test
public:
	CSector();
	CSector(int _x, int _y);
	~CSector();

	void SetAdjacentSector(CSector* _pSector);
	std::vector<CSector*> Difference(sector_t _b);
	void DeleteUsersOutOfSector(CUser& _user);
	void FetchUserInfoInNewSector(CUser& _user);

	void Add(CUser* _user);
	void Add(CMonster* _pMonster);
	void Del(CUser* _user);
	void Del(CMonster* _pMonster);
	void Send(char* _buffer, int _size);
	void SendAll(char* _buffer, int _size);
	
	int Size();
	int GetSectorUserCount();
	std::map<SOCKET, CUser*> GetMap();
	std::map<SOCKET, CUser*> GetAdjacentGetMap(int _index);
	std::vector<CSector*> GetAdjacentSector();
	int GetAdjacentUserCount(int _index);
};