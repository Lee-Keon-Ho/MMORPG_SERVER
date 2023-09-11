#pragma once
#include <Windows.h>

class CRingBuffer
{
private:
	int		m_size;

	char*	m_buffer;
	char*	m_tempBuffer;
	char*	m_pRead;
	char*	m_pWrite;
	char*	m_pBufferEnd;

	int		m_remainDataSize;

	CRITICAL_SECTION m_cs;
public:
	CRingBuffer(int _bufferSize = 65530);
	virtual ~CRingBuffer();

	int GetWriteBufferSize();	

	void Write(int _size);

	int GetReadSize();

	void Read(int _size);

	bool IsFull();

	char* GetBuffer();

	int GetRemainDataSize();

	char* GetEndBufferAddr();

	char* GetWriteBuffer();

	char* GetReadBuffer();

	int GetRemainSize_EndBuffer(const char* _pBuffer);

	char* GetPacketBuffer();
};