#pragma once
#include "../NetCore/TcpListener.h"
#include "../NetCore/ThreadManager.h"
#include "App.h"

class CFieldApp : public CApp
{
private:
	CTcpListener* m_pListener;
	CThreadManager* m_pThreadManager;

public:
	CFieldApp();
	~CFieldApp();

public:
	bool Initialize() override;
	bool CreateInstance() override;
	bool StartInstance() override;
	void RunLoop() override;
	void DeleteInstance() override;
};