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
		{ L"test1 중 입니다." },
		{ L"test2 중 입니다." },
		{ L"test3 중 입니다." },
		{ L"test4 중 입니다." },
		{ L"test5 중 입니다." },
		{ L"test6 중 입니다." },
		{ L"test7 중 입니다." },
		{ L"test8 중 입니다." },
		{ L"test9 중 입니다." },
		{ L"test10 중 입니다." },
	};

private:
	CNotice();
	~CNotice();

public:
	void Start();
	void RunLoop() override;
};