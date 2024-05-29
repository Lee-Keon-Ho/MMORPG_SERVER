#pragma once
#include "ODBC.h"
#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>

class CODBCPool
{
private:
	SQLWCHAR* m_dbName;
	int									m_poolSize;
	std::vector<std::unique_ptr<CODBC>>	m_connections;
	std::mutex							m_mutex;
	std::condition_variable				m_condition;

public:
	CODBCPool(SQLWCHAR* _dbName, int _poolSize);
	~CODBCPool();

	std::unique_ptr<CODBC> GetConnection();
	void ReleaseConnection(std::unique_ptr<CODBC> _conn);
};