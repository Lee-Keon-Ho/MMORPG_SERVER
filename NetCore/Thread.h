#pragma once

class CThread
{
public:
	CThread();
	~CThread();

	bool Start();
	static unsigned int _stdcall ThreadFunc(void* _pArgs);
	virtual void RunLoop() = 0;
};