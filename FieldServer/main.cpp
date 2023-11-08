#include "FieldApp.h"
#include <stdio.h>
#include <iostream>
#include <exception>
int main(void)
{
	//long a = 10;
	//int b = InterlockedIncrement(&a); // fieldServer
	//std::atomic<int> c; // loginServer
	//volatile long d = 0;

	CFieldApp app;
	if (app.Init())
	{
		app.Run();
	}
	app.Destroy();
	
	return 0;
}