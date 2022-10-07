#pragma once

#define _WIN32_DCOM

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <comdef.h>
#include <wincred.h>
//  Include the task header file.
#include <taskschd.h>
#include <shlwapi.h>

#include <string.h>
#include <strsafe.h>
#include <random>

#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsupp.lib")
#pragma comment(lib, "credui.lib")

#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "Shlwapi.lib")

#include "ValidIOHandlers.h"
#include "Remainder.h"
#include "CreateTask.h"

using namespace std;

class UpdateTask
{
public:
	UpdateTask() {
		validIOHandlers = new ValidIOHandlers();
		remainder = new Remainder();
		createTask = new CreateTask();
	}
	
	int updateEvent();

	HRESULT updateTriggers(HRESULT& hr, ITaskDefinition* pTask, ITaskFolder* pRootFolder, ITriggerCollection* pTriggerCollection);

private:
	ValidIOHandlers* validIOHandlers;
	Remainder* remainder;
	CreateTask* createTask;
};

