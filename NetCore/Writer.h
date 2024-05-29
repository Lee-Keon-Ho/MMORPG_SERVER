#pragma once
#include "LStruct.h"
#include "sharedPtr.h"
#include <deque>

class CWriter
{
private:
	std::deque<LKH::sharedPtr<PACKET>>	m_que;
	PSRWLOCK							m_lock;
	WSABUF								m_dataBuf;
public:
	CWriter();
	~CWriter();

	void Write(LKH::sharedPtr<PACKET> _packet);
	void SetBuffer();
	void SetWriteLenZero();
	bool IsWriteBuffer();
	bool IsWriteLenZero();
	void HandleUsedBuffer();
	LPWSABUF GetWSABUF();
};