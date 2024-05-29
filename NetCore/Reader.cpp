#include "Reader.h"
#include <assert.h>
CReader::CReader(int _bufferSize) :
	m_pRingBuffer(nullptr)
{
	m_pRingBuffer = std::make_unique<CRingBuffer>(_bufferSize);
	assert(m_pRingBuffer != nullptr);
	m_dataBuf.buf = m_pRingBuffer->GetWriteBuffer();
	m_dataBuf.len = m_pRingBuffer->GetWriteBufferSize();
}

CReader::~CReader()
{
}

void CReader::ConsumeSize(DWORD _size)
{
	m_pRingBuffer.get()->Write(_size);

	m_dataBuf.len = m_pRingBuffer.get()->GetWriteBufferSize();
	m_dataBuf.buf = m_pRingBuffer.get()->GetWriteBuffer();
}

LPWSABUF CReader::GetWSABUF()
{
	return &m_dataBuf;
}

char* CReader::GetBuffer()
{
	return m_pRingBuffer.get()->GetPacketBuffer();
}

int CReader::GetReadSize()
{
	return m_pRingBuffer.get()->GetReadSize();;
}

void CReader::Read(int _size)
{
	m_pRingBuffer.get()->Read(_size);
}
