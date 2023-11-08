#pragma once
#include "../NetCore/Thread.h"
#include "../NetCore/RingBuffer.h"

class CLoginConnector : public CThread
{
private:
	SOCKET		m_socket;
	CRingBuffer* m_pRingBuffer;
public:
	CLoginConnector(PCSTR _ip, u_short _port);
	~CLoginConnector();

	bool Start();
	void RunLoop() override;
};