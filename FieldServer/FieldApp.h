#pragma once
#include "../NetCore/TcpListener.h"
#include "../NetCore/ThreadManager.h"
#include "App.h"
#include "FieldServerAccept.h"
#include <vector>

class CFieldApp : public CApp
{
public:
	typedef std::vector<CUser*> t_user; // ���� �ȳ����� vector�� ���� // ���� : �ʿ����

private:
	CTcpListener* m_pListener;
	CThreadManager* m_pThreadManager;
	CFieldServerAccept* m_pFieldServerAccept;

	t_user m_user; // �ʿ����
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