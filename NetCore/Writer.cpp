#include "Writer.h"
#include "Lock.h"

CWriter::CWriter() : m_lock(new SRWLOCK())
{
	InitializeSRWLock(m_lock);
}

CWriter::~CWriter()
{
	if (m_lock) { delete m_lock; m_lock = nullptr; }
}

void CWriter::Write(LKH::sharedPtr<PACKET> _packet)
{
	CLock lock(m_lock, eLockType::EXCLUSIVE);
	m_que.push_back(_packet);
}

void CWriter::SetBuffer()
{
	CLock lock(m_lock, eLockType::SHARED);
	m_dataBuf.len = m_que.front().get()->size;
	m_dataBuf.buf = reinterpret_cast<char*>(m_que.front().get());
}

void CWriter::SetWriteLenZero()
{
	m_dataBuf.len = 0;
}

bool CWriter::IsWriteBuffer()
{
	return m_que.size() > 0;
}

bool CWriter::IsWriteLenZero()
{
	return m_dataBuf.len == 0;
}

void CWriter::HandleUsedBuffer()
{
	CLock lock(m_lock, eLockType::EXCLUSIVE);
	m_que.pop_front();
}

LPWSABUF CWriter::GetWSABUF()
{
	return &m_dataBuf;
}
