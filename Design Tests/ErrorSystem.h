#pragma once
#ifndef DEBUG_MODE
#define DEBUG_MODE true
#endif // !DEBUG_MODE

//==========================================================================================
// File Name: ErrorSystem.h
// Author: Brian Blackmon
// Date Created: 3/19/2019
// Purpose: 
// Handles the output of errors. Uses the singleton pattern to ensure there's only one 
// system running at a time.
//==========================================================================================

class ErrorSystem
{
public:
	~ErrorSystem();

	static ErrorSystem& instance()
	{
		static ErrorSystem *system = new ErrorSystem();

		return *system;
	}

	static void logError(const char *error);

private:
	ErrorSystem();
};

