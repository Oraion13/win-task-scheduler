#pragma once
//  Include the task header file.
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

