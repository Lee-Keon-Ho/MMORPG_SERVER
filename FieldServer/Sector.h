#pragma once
#include "User.h"
#include "Monster.h"
#include "../NetCore/Lock.h"
#include "PacketStruct.h"
#include "PacketType.h"
#include <map>
#include <vector>

#define SECTOR_SIZE 18
#define SECTOR_LINE 15

using namespace std;

class CSector
{
private:
	typedef map<SOCKET, CUser*> userMap_t;
	typedef vector<CSector*> sector_t;
	typedef map<int, CMonster*> monster_t;
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
	vector<CSector*> Difference(sector_t _b);
	void WrapUser(CUser& _pUser);
	void WrapDeleteUser(CUser& _pUser);
	void DeleteUsersOutOfSector(CUser& _user);
	void FetchUserInfoInNewSector(CUser& _user);
	void DeleteMonstersOutOfSector(CUser& _user);
	void FetchMonsterInfoInNewSector(CUser& _user);

	void Add(CUser* _user);
	void Add(CMonster* _pMonster);
	void Del(CUser* _user);
	void Del(CMonster* _pMonster);
	void Send(char* _buffer, int _size);
	void SendAll(char* _buffer, int _size);
	
	int Size();
	int GetSectorUserCount();
	map<SOCKET, CUser*> GetMap();
	map<SOCKET, CUser*> GetAdjacentGetMap(int _index);
	vector<CSector*> GetAdjacentSector();
	int GetAdjacentUserCount(int _index);
	void SendMonsterInfo(CUser* _pUser);
	void SendAdjacentSectorMonsterInfo(CUser* _pUser);
};