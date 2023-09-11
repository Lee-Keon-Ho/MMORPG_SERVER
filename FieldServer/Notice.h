#pragma once
#include "../NetCore/Thread.h"
#include <iostream>
#include <string>
#include <vector>

#define MAX_ARRAY_SIZE 10
#define MAX_CHAR_SIZE 30

using namespace std;

class CNotice : public CThread
{
public:
	static CNotice* GetInstance()
	{
		static CNotice pInstance;
		return &pInstance;
	}

private:
	vector<string> m_strList;

	wchar_t m_list[MAX_ARRAY_SIZE][MAX_CHAR_SIZE] =
	{
		{ L"test1 �� �Դϴ�." },
		{ L"test2 �� �Դϴ�." },
		{ L"test3 �� �Դϴ�." },
		{ L"test4 �� �Դϴ�." },
		{ L"test5 �� �Դϴ�." },
		{ L"test6 �� �Դϴ�." },
		{ L"test7 �� �Դϴ�." },
		{ L"test8 �� �Դϴ�." },
		{ L"test9 �� �Դϴ�." },
		{ L"test10 �� �Դϴ�." },
	};

private:
	CNotice();
	~CNotice();

public:
	void Start();
	void RunLoop() override;
};