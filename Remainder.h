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
	int updateEvent();
	int deleteEvent();

	BSTR ConvertMBSToBSTR(const string& str);

private:
	ValidIOHandlers* validIOHandlers;
	unordered_map<int, string>* recurrence;
	RemainderManagement* remainderManagement;
}
