#include "ErrorSystem.h"
#include <stdio.h>


ErrorSystem::ErrorSystem()
{

}


ErrorSystem::~ErrorSystem()
{

}

void ErrorSystem::logError(const char *error)
{
	if(DEBUG_MODE)
	{
		printf("Error occurred: %s\n", error);
	}
}