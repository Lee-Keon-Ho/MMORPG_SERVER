#pragma once
#include "../NetCore/ThreadManager.h"
#include "App.h"
#include "FieldAcceptor.h"
#include "Map.h"
class CFieldApp : public CApp
{
private:
	CThreadManager* m_pThreadManager;
	CFieldAcceptor* m_pFieldAcceptor;
	CMap*			m_pMap; // field
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