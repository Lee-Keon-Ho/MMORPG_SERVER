#include "FieldApp.h"
#include <stdio.h>

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