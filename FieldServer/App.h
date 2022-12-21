#pragma once
#include "../NetCore/TcpListener.h"
class CApp
{
private:
	CTcpListener* m_pListener;

public:
	CApp();
	~CApp();

	bool Initialize();
	void Run();
	void Destroy();
	void CreateInstance();
	void StartInstance();

	ACCEPT_SOCKET_INFO Accept();
};