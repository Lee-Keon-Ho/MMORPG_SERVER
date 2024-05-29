#pragma once
#include <WinSock2.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include <string>

class CODBC
{
private:
	SQLHENV		m_hEnv;
	SQLHDBC		m_hDbc;

public:
	CODBC();
	CODBC(SQLWCHAR* _dbName);
	~CODBC();

	void Update_Level_and_Exp(std::wstring _query);
	void Update_Field(std::wstring _query);
	void Update_Position(std::wstring _query);
	void MAX_EXP(std::wstring _query, float& _max_exp);
	void Close();
};