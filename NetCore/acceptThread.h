#pragma once
#include <WinSock2.h>

class CAcceptThread // CThread가 맞다 생각을 좀 더 하자
{
protected:
	HANDLE m_threadId;

public:
	CAcceptThread();
	~CAcceptThread();

	bool tStart(); // 이름을 바꾸자
	static unsigned int _stdcall AcceptFunc(void* _pArgs);
	virtual void RunLoop() = 0;
};