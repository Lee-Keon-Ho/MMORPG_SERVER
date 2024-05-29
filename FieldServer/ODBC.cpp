#include "ODBC.h"
#include <stdexcept>
#include <iostream>

CODBC::CODBC() :
	m_hEnv(nullptr),
	m_hDbc(nullptr)
{
}

CODBC::CODBC(SQLWCHAR* _dbName) :
	m_hEnv(nullptr),
	m_hDbc(nullptr)
{
	// ODBC 환경 핸들 생성
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv) != SQL_SUCCESS)
	{
		throw std::runtime_error("SQLAlloc Error");
	}

	// ODBC 버전 설정
	if (SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0) != SQL_SUCCESS)
	{
		throw std::runtime_error("SQLEnv Error");
	}

	// DB 연결 핸들 생성
	if (SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc) != SQL_SUCCESS)
	{
		throw std::runtime_error("DB Handle Error");
	}

	// DB에 연결
	if (SQLConnectW(m_hDbc, _dbName, SQL_NTS, nullptr, SQL_NTS, nullptr, SQL_NTS) == SQL_ERROR)
	{
		throw std::runtime_error("DB connect Error");
	}
}

CODBC::~CODBC()
{
}

void CODBC::Update_Level_and_Exp(std::wstring _query)
{
	SQLHSTMT stmt;
	if (SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &stmt) != SQL_SUCCESS)
	{
		// 오류 처리
	}
	if (SQLExecDirectW(stmt, (SQLWCHAR*)_query.c_str(), SQL_NTS) != SQL_SUCCESS)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		return;
	}
	SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}

void CODBC::Update_Field(std::wstring _query)
{
	SQLHSTMT stmt;
	if (SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &stmt) != SQL_SUCCESS) return;
	if (SQLExecDirectW(stmt, (SQLWCHAR*)_query.c_str(), SQL_NTS) != SQL_SUCCESS)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		return;
	}

	if (SQLFetch(stmt) != SQL_SUCCESS)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		return;
	}
}

void CODBC::Update_Position(std::wstring _query)
{
	SQLHSTMT stmt;
	if (SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &stmt) != SQL_SUCCESS) return;
	if (SQLExecDirectW(stmt, (SQLWCHAR*)_query.c_str(), SQL_NTS) != SQL_SUCCESS)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		return;
	}

	if (SQLFetch(stmt) != SQL_SUCCESS)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		return;
	}
}

void CODBC::MAX_EXP(std::wstring _query, float& _max_exp)
{
	SQLHSTMT stmt;
	int exp;
	if (SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &stmt) != SQL_SUCCESS) return;
	if (SQLExecDirectW(stmt, (SQLWCHAR*)_query.c_str(), SQL_NTS) != SQL_SUCCESS)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		return;
	}

	if (SQLFetch(stmt) != SQL_SUCCESS)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		return;
	}

	SQLGetData(stmt, 1, SQL_INTEGER, &exp, sizeof(SQLINTEGER), NULL);
	_max_exp = exp;
}
