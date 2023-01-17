#pragma once
#include "../NetCore/session.h"

class CUser : public CSession
{
private:
	bool m_bLogOut;
public:
	CUser();
	CUser(ACCEPT_SOCKET_INFO _socketInfo);
	~CUser();

	int PacketHandle();
	void SetLogOut(bool _bLogOut) { m_bLogOut = _bLogOut; }
	bool GetLogOut() { return m_bLogOut; }
};