#pragma once

#include "ValidIOHandlers.h"
#include "Remainder.h"
#include "CreateTask.h"

#define _WIN32_DCOM

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <comdef.h>
#include <wincred.h>
#include <random>
//  Include the task header file.
#include <taskschd.h>
#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsupp.lib")
#pragma comment(lib, "credui.lib")


#include <tchar.h>
#include <strsafe.h>
#include <shlwapi.h>

#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "Shlwapi.lib")

#include <atlcomtime.h>

#include <initguid.h>
#include <ole2.h>
#include <mstask.h>
#include <msterr.h>
#include <wchar.h>

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

