#pragma once
#include "ValidIOHandlers.h"
#include "Remainder.h"

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

class CreateTask
{
public:
	CreateTask() {
		validIOHandlers = new ValidIOHandlers();
		remainder = new Remainder();
	}

	int createEvent();

	HRESULT createTimeTrigger(HRESULT& hr, ITrigger* pTrigger, ITaskFolder* pRootFolder, ITaskDefinition* pTask);
	HRESULT createDailyTrigger(HRESULT& hr, ITrigger* pTrigger, ITaskFolder* pRootFolder, ITaskDefinition* pTask);
	HRESULT createWeeklyTrigger(HRESULT& hr, ITrigger* pTrigger, ITaskFolder* pRootFolder, ITaskDefinition* pTask);
	HRESULT createMonthlyTrigger(HRESULT& hr, ITrigger* pTrigger, ITaskFolder* pRootFolder, ITaskDefinition* pTask);

	HRESULT registrationInfo(HRESULT& hr, ITaskDefinition* pTask, IRegistrationInfo* pRegInfo, ITaskFolder* pRootFolder);
	HRESULT principal(HRESULT& hr, ITaskDefinition* pTask, IPrincipal* pPrincipal, ITaskFolder* pRootFolder);
	HRESULT settings(HRESULT& hr, ITaskDefinition* pTask, ITaskSettings* pSettings, ITaskFolder* pRootFolder);
	HRESULT trigger(HRESULT& hr, ITaskDefinition* pTask, ITriggerCollection* pTriggerCollection, ITrigger* pTrigger, ITaskFolder* pRootFolder, bool isNewTrigger);

private:
	ValidIOHandlers* validIOHandlers;
	Remainder* remainder;

	string getUUID(size_t len);
};

