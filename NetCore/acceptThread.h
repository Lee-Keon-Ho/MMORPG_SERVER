#pragma once
#include <WinSock2.h>

class CAcceptThread // CThread�� �´� ������ �� �� ����
{
protected:
	HANDLE m_threadId;

public:
	CAcceptThread();
	~CAcceptThread();

	bool tStart(); // �̸��� �ٲ���
	static unsigned int _stdcall AcceptFunc(void* _pArgs);
	virtual void RunLoop() = 0;
};