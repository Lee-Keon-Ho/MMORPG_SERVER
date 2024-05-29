#pragma once
#include "User.h"
#include <iostream>
#include <windows.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include "ODBCPool.h"

class CODBCManager
{
public:
	static CODBCManager* GetInstance()
	{
		static CODBCManager instance;
		return &instance;
	}

private:
	CODBCManager();
	~CODBCManager();


private:
	std::unique_ptr<CODBCPool> m_pConnectionPool;
	std::unique_ptr<CODBCPool> m_pExpPool;

public:
	bool Initialize(SQLWCHAR* _db);
	void Update_Field(CUser& _pUser, u_short _field);
	void Update_Level_and_Exp(CUser& _pUser);
	void Update_Position(CUser& _pUser);
	void MAX_EXP(CUser& _pUser, float& _exp);
};