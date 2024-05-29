#pragma once
#include "../NetCore/ThreadManager.h"
#include "App.h"
#include "FieldAcceptor.h"
#include "LoginConnector.h"
#include "MonitorAcceptor.h"
#include "Map.h"
#include "HeartBeat.h"

class CFieldApp : public CApp
{
private:
	std::unique_ptr<CThreadManager>		m_pThreadManager;
	std::unique_ptr<CFieldAcceptor>		m_pFieldAcceptor;
	std::unique_ptr<CLoginConnector>	m_pLoginConnector;
	std::unique_ptr<CMonitorAcceptor>	m_pMonitorAcceptor;
	std::unique_ptr<CHeartBeat>			m_pHeartBeat;

	HANDLE								m_hEvent;
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