#pragma once
#include <windows.h>

class CLock
{
private:
	CRITICAL_SECTION m_cs;
public:
	CLock(CRITICAL_SECTION _cs);
	~CLock();

};