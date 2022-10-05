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
		feedRecurrence();
		validIOHandlers = new ValidIOHandlers();
		remainderManagement = new RemainderManagement();
	}

	int createEvent();
	//void readEvent();
	//void updateEvent();
	//void deleteEvent();

private:
	ValidIOHandlers* validIOHandlers;
	unordered_map<int, string>* recurrence;
	RemainderManagement* remainderManagement;

	void feedRecurrence();
	void displayRecurrence();
	string getRecurrence(int key);
	string getUUID(size_t len);

	HRESULT createTimeTrigger(HRESULT& hr, ITrigger* pTrigger, ITaskFolder* pRootFolder, ITaskDefinition* pTask);
	HRESULT createDailyTrigger(HRESULT& hr, ITrigger* pTrigger, ITaskFolder* pRootFolder, ITaskDefinition* pTask);

	BSTR ConvertMBSToBSTR(const string& str);
};

