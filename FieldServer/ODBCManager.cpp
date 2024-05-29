#include "ODBCManager.h"
#include <assert.h>
CODBCManager::CODBCManager() : 
	m_pConnectionPool(nullptr),
	m_pExpPool(nullptr)
{
}

CODBCManager::~CODBCManager()
{
}

bool CODBCManager::Initialize(SQLWCHAR* _db)
{
    if (!m_pConnectionPool) m_pConnectionPool = std::make_unique<CODBCPool>(_db, 10);
    assert(m_pConnectionPool != nullptr);
	if (!m_pExpPool) m_pExpPool = std::make_unique<CODBCPool>((SQLWCHAR*)L"Experience", 10);
	assert(m_pExpPool != nullptr);
    return true;
}

void CODBCManager::Update_Field(CUser& _pUser, u_short _field)
{
	VECTOR3 position = _pUser.GetCharacterInfo().position;
	std::unique_ptr<CODBC> conn = m_pConnectionPool->GetConnection();
	std::wstring str = L"update CharacterInfo set FIELD = ";
	conn.get()->Update_Field(str + std::to_wstring(_field) + L", POSITION_X = " + std::to_wstring(position.x) + L", POSITION_Z = " + std::to_wstring(position.z) + L" where NAME = '" + _pUser.GetCharacterInfo().name + L"'");
	m_pConnectionPool->ReleaseConnection(std::move(conn));
}

void CODBCManager::Update_Level_and_Exp(CUser& _pUser)
{
	std::unique_ptr<CODBC> conn = m_pConnectionPool->GetConnection();
	std::wstring str = L"update CharacterInfo set CUR_EXP = ";
	conn->Update_Level_and_Exp(str + std::to_wstring(_pUser.GetCharacterInfo().curExp) + L", LEVEL = " + std::to_wstring(_pUser.GetCharacterInfo().level) + L" where NAME = '" + _pUser.GetCharacterInfo().name + L"'");
	m_pConnectionPool->ReleaseConnection(std::move(conn));

	conn = m_pConnectionPool->GetConnection();
	std::wstring Liststr = L"update CharacterList set level = ";
	conn->Update_Level_and_Exp(Liststr + std::to_wstring(_pUser.GetCharacterInfo().level) + L" where NAME = '" + _pUser.GetCharacterInfo().name + L"'");
	m_pConnectionPool->ReleaseConnection(std::move(conn));
}

void CODBCManager::Update_Position(CUser& _pUser)
{
	VECTOR3 position = _pUser.GetCharacterInfo().position;

	std::unique_ptr<CODBC> conn = m_pConnectionPool->GetConnection();
	std::wstring str = L"update CharacterInfo set POSITION_X = ";
	conn.get()->Update_Field(str + std::to_wstring(position.x) + L", POSITION_Y = " + std::to_wstring(position.y) + L", POSITION_Z = " + std::to_wstring(position.z) + L" where NAME = '" + _pUser.GetCharacterInfo().name + L"'");
	m_pConnectionPool->ReleaseConnection(std::move(conn));
}

void CODBCManager::MAX_EXP(CUser& _pUser, float& _exp)
{
	std::unique_ptr<CODBC> conn = m_pExpPool->GetConnection();
	std::wstring str = L"select necessary from expTable where level = ";
	conn->MAX_EXP(str + std::to_wstring(_pUser.GetCharacterInfo().level), _exp);
	m_pExpPool->ReleaseConnection(std::move(conn));
}

void CODBC::Close()
{
	SQLDisconnect(m_hDbc);
	SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
}
