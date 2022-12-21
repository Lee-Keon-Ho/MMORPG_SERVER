#pragma once

class CRingBuffer
{
private:
	int m_size;

	char* m_buffer;
	char* m_pRead;
	char* m_pWrite;
	char* m_pBufferEnd;

	int m_remainDataSize;
public:
	CRingBuffer(int _bufferSize = 65530);
	virtual ~CRingBuffer();

	int GetWriteBufferSize();	

	void Write(int _size);

	int GetReadSize();

	void Read(int _size);

	bool IsFull() { return (m_remainDataSize >= m_size); }

	char* GetBuffer() { return m_buffer; }

	int GetRemainDataSize() { return m_remainDataSize; }

	char* GetEndBufferAddr() { return m_pBufferEnd; }

	char* GetWriteBuffer() { return m_pWrite; }

	char* GetReadBuffer() { return m_pRead; }

	int GetRemainSize_EndBuffer(const char* _pBuffer) { return (int)(m_pBufferEnd - _pBuffer); }
};