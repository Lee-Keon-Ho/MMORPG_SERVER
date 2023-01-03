#pragma once
#include "../NetCore/overlapped_ex.h"

class CUser : public CSession
{
private:
	overlapped_ex m_overlapped;

public:
	CUser();
	CUser(ACCEPT_SOCKET_INFO _socketInfo);
	~CUser();

	bool Recv() override;
	int PacketHandle();
	char* GetPacketBuffer();
};