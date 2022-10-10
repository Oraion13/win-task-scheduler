#pragma once
#include <iostream>
#include <string>
#include <unordered_map>

#include <vector>

#include "ValidIOHandlers.h"

#define _WIN32_DCOM

#include <windows.h>
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

class Remainder
{
public:
	Remainder() {
		validIOHandlers = new ValidIOHandlers();
	}

	int createEvent();
	int readEvent();
	int updateEvent();
	int deleteEvent();

	BSTR ConvertMBSToBSTR(const string& str);
	string ConvertWCSToMBS(const wchar_t* pstr, long wslen);
	string callConvertWCSToMBS(BSTR* bstr);

private:
	ValidIOHandlers* validIOHandlers;
	unordered_map<int, string>* recurrence;
};