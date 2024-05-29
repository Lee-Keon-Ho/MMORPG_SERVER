#include "ODBCPool.h"
#include "../NetCore/Lock.h"
#include <iostream>

CODBCPool::CODBCPool(SQLWCHAR* _dbName, int _poolSize) :
	m_dbName(_dbName),
	m_poolSize(_poolSize)
{
	try
	{
		for (int i = 0; i < m_poolSize; ++i)
		{
			std::unique_ptr<CODBC> conn = std::make_unique<CODBC>(m_dbName);
			m_connections.push_back(std::move(conn));
		}
	}
	catch (std::runtime_error e)
	{
		std::cout << e.what() << std::endl;
	}
}

CODBCPool::~CODBCPool()
{

}

std::unique_ptr<CODBC> CODBCPool::GetConnection()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	while (m_connections.empty())
	{
		m_condition.wait(lock);
	}
	std::unique_ptr<CODBC> conn = std::move(m_connections.back());
	m_connections.pop_back();
	return std::move(conn);
}

void CODBCPool::ReleaseConnection(std::unique_ptr<CODBC> _conn)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_connections.push_back(std::move(_conn));
	m_condition.notify_one();
}
