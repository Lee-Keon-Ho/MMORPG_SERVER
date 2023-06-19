#pragma once
#include "User.h"
#include "PacketStruct.h"
#include "PacketType.h"
#include <map>
#include <vector>
class CSector
{
private:
	typedef std::map<SOCKET, CUser*> userMap_t;
	typedef std::vector<CSector*> sector_t;
private:
	userMap_t m_userList;

	CRITICAL_SECTION m_cs_user;

	sector_t m_AdjacentSector;

	int nSector;
	
public:
	CSector();
	CSector(int _x, int _y);
	~CSector();

	void SetAdjacentSector(CSector* _pSector);
	std::vector<CSector*> Difference(sector_t _b);
	void DeleteUsersOutOfSector(CUser& _user);
	void FetchUserInfoInNewSector(CUser& _user);

	void Add(CUser* _user);
	void Del(CUser* _user);
	void Send(char* _buffer, int _size);
	void SendAll(char* _buffer, int _size);
	
	int Size();
	int GetSectorUserCount();
	std::map<SOCKET, CUser*> GetMap();
	std::map<SOCKET, CUser*> GetAdjacentGetMap(int _index);
	std::vector<CSector*> GetAdjacentSector();
	int GetAdjacentUserCount(int _index);
};