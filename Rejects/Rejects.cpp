#include "Rejects.h"

/*
	To test the library, include "Rejects.h" from an application project
	and call RejectsTest().
	
	Do not forget to add the library to Project Dependencies in Visual Studio.
*/

static int s_Test = 0;

extern "C" int RejectsTest();

int RejectsTest()
{
	return ++s_Test;
}