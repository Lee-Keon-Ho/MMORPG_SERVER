#include "RingBuffer.h"
#include "Lock.h"
#include <memory>

CRingBuffer::CRingBuffer(int _bufferSize) : m_size(_bufferSize), m_remainDataSize(0)
{
	m_buffer = new char[m_size];
	m_tempBuffer = new char[m_size];
	memset(m_buffer, 0, sizeof(char) * m_size);

	m_pRead = m_buffer;
	m_pWrite = m_buffer;
	m_pBufferEnd = m_buffer + m_size;

	InitializeCriticalSection(&m_cs);
}

CRingBuffer::~CRingBuffer()
{
	DeleteCriticalSection(&m_cs);
	if (m_tempBuffer) { delete m_tempBuffer; m_tempBuffer = nullptr; }
	if (m_buffer) { delete m_buffer; m_buffer = nullptr; }
}

int CRingBuffer::GetWriteBufferSize()
{
	if (IsFull()) return 0;

	if (m_pWrite >= m_pRead) return GetRemainSize_EndBuffer(m_pWrite);

	return (int)(m_pRead - m_pWrite);
}

void CRingBuffer::Write(int _size)
{
	CLock lock(m_cs);
	m_pWrite += _size;
	m_remainDataSize += _size;

	if (m_pWrite == m_pBufferEnd) m_pWrite = m_buffer;
}

int CRingBuffer::GetReadSize()
{
	if (m_remainDataSize == 0) return 0;

	int size;

	if (GetRemainSize_EndBuffer(m_pRead) == 1)
	{
		if (m_remainDataSize == 1) return 0;

		char tempBuf[100];
		memcpy(tempBuf, m_pRead, 1);
		memcpy(tempBuf + 1, m_buffer, 1);

		size = *(unsigned short*)tempBuf;
	}
	else size = *(unsigned short*)m_pRead;

	if (m_remainDataSize >= size) return size;

	return 0;
}

void CRingBuffer::Read(int _size)
{
	CLock lock(m_cs);
	if (m_remainDataSize >= _size)
	{
		int endBuf_Read = GetRemainSize_EndBuffer(m_pRead);

		if (endBuf_Read < _size)
		{
			endBuf_Read = _size - endBuf_Read;
			m_pRead = m_buffer;
			m_pRead += endBuf_Read;
		}
		else
		{
			m_pRead += _size;
			if (m_pRead == m_pBufferEnd) m_pRead = m_buffer;
		}

		m_remainDataSize -= _size;
	}
}

bool CRingBuffer::IsFull()
{
	return (m_remainDataSize >= m_size);
}

char* CRingBuffer::GetBuffer()
{
	return m_buffer;
}

int CRingBuffer::GetRemainDataSize()
{
	return m_remainDataSize;
}

char* CRingBuffer::GetEndBufferAddr()
{
	return m_pBufferEnd;
}

char* CRingBuffer::GetWriteBuffer()
{
	return m_pWrite;
}

char* CRingBuffer::GetReadBuffer()
{
	return m_pRead;
}

int CRingBuffer::GetRemainSize_EndBuffer(const char* _pBuffer)
{
	return static_cast<int>(m_pBufferEnd - _pBuffer);
}

char* CRingBuffer::GetPacketBuffer()
{
	char* readBuffer = m_pRead;
	int read_EndBuf = GetRemainSize_EndBuffer(readBuffer);
	int readSize = GetReadSize();

	if (readSize > read_EndBuf)
	{
		memcpy(m_tempBuffer, readBuffer, read_EndBuf);
		memcpy(m_tempBuffer + read_EndBuf, m_buffer, readSize - read_EndBuf);
		readBuffer = m_tempBuffer;
	}

	if (readSize == 0) return nullptr;

	return readBuffer;
}