#pragma once
#include <WinSock2.h>
#include "RingBuffer.h"
#include <iostream>

class CReader
{
private:
	std::unique_ptr<CRingBuffer>	m_pRingBuffer;
	WSABUF							m_dataBuf;
public:
	CReader(int _bufferSize = 65530);
	~CReader();

	void Read(int _size);
	void ConsumeSize(DWORD _size);
	LPWSABUF GetWSABUF();
	char* GetBuffer();
	int GetReadSize();
};