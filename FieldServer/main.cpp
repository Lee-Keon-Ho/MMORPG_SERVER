#include "FieldApp.h"
#include <stdio.h>
#include <iostream>
#include <exception>
int main(void)
{
	CFieldApp app;
	if (app.Init())
	{
		app.Run();
	}
	app.Destroy();
	
	return 0;
}