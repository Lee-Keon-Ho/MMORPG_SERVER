#pragma once
#include "../NetCore/ThreadManager.h"
#include "App.h"
#include "FieldAcceptor.h"
#include <list>

class CFieldApp : public CApp
{
public:
	typedef std::list<CUser*> t_user; // 절대 안나가면 vector가 좋다 // 수업 : 필요없다

private:
	CThreadManager* m_pThreadManager;
	CFieldAcceptor* m_pFieldAcceptor;

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