#include "App.h"
#include <stdio.h>

int main(void)
{
	CApp app;

	app.Initialize();
	app.CreateInstance();
	app.StartInstance();
}