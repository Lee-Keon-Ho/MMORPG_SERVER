#pragma once
#include "../NetCore/TcpListener.h"
#include "App.h"

class CFieldApp : public CApp
{
private:
	CTcpListener* m_pListener;

public:
	CFieldApp();
	~CFieldApp();

	ACCEPT_SOCKET_INFO Accept();
	bool ThreadStart();
public:
	bool Initialize() override;
	bool CreateInstance() override;
	bool StartInstance() override;
	void RunLoop() override;
	void DeleteInstance() override;
};