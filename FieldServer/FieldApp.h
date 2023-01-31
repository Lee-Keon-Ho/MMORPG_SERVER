#pragma once
#include "../NetCore/ThreadManager.h"
#include "App.h"
#include "FieldAcceptor.h"
#include <list>

class CFieldApp : public CApp
{
private:
	CThreadManager* m_pThreadManager;
	CFieldAcceptor* m_pFieldAcceptor;

public:
	CFieldApp();
	~CFieldApp();

public:
	bool Initialize() override;
	bool CreateInstance() override;
	bool StartInstance() override;
	void RunLoop() override;
	void DeleteInstance() override;

	void Handle(ACCEPT_SOCKET_INFO);
};