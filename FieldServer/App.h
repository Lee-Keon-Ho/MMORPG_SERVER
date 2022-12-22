#pragma once

class CApp
{
public:
	CApp();
	virtual ~CApp();

	bool Init();
	void Run();
	void Destroy();

protected:
	virtual bool Initialize() = 0;
	virtual bool CreateInstance() = 0;
	virtual bool StartInstance() = 0;
	virtual void RunLoop() {}
	virtual void DeleteInstance() {}
};