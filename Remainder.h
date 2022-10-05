#pragma once
#include <iostream>
#include <string>
#include <unordered_map>

#include "ValidIOHandlers.h"
#include "RemainderManagement.h"

using namespace std;

class Remainder
{
public:
	Remainder() {
		validIOHandlers = new ValidIOHandlers();
		remainderManagement = new RemainderManagement();
	}

	int createEvent();
	int readEvent();
	//void updateEvent();
	int deleteEvent();

private:
	ValidIOHandlers* validIOHandlers;
	unordered_map<int, string>* recurrence;
	RemainderManagement* remainderManagement;

	string getUUID(size_t len);

	HRESULT createTimeTrigger(HRESULT& hr, ITrigger* pTrigger, ITaskFolder* pRootFolder, ITaskDefinition* pTask);
	HRESULT createDailyTrigger(HRESULT& hr, ITrigger* pTrigger, ITaskFolder* pRootFolder, ITaskDefinition* pTask);
	HRESULT createWeeklyTrigger(HRESULT& hr, ITrigger* pTrigger, ITaskFolder* pRootFolder, ITaskDefinition* pTask);
	HRESULT createMonthlyTrigger(HRESULT& hr, ITrigger* pTrigger, ITaskFolder* pRootFolder, ITaskDefinition* pTask);

	BSTR ConvertMBSToBSTR(const string& str);
};

