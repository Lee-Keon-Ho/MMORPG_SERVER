#pragma once
#include "../NetCore/TcpListener.h"
#include "../NetCore/ThreadManager.h"
#include "App.h"
#include "FieldServerAccept.h"
#include <vector>

class CFieldApp : public CApp
{
public:
	typedef std::vector<CUser*> t_user; // 절대 안나가면 vector가 좋다 // 수업 : 필요없다

private:
	CTcpListener* m_pListener;
	CThreadManager* m_pThreadManager;
	CFieldServerAccept* m_pFieldServerAccept;

	t_user m_user; // 필요없다
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