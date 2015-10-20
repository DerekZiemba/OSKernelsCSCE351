#include "SharedResources.h"

/*
	To test the library, include "SharedResources.h" from an application project
	and call SharedResourcesTest().
	
	Do not forget to add the library to Project Dependencies in Visual Studio.
*/

static int s_Test = 0;

extern "C" int SharedResourcesTest();

int SharedResourcesTest()
{
	return ++s_Test;
}