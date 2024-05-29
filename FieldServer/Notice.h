#pragma once
#include "../NetCore/Thread.h"
#include "constants.h"
#include <iostream>
#include <string>
#include <vector>

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

	wchar_t m_list[array_max_size][chat_max_size] =
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