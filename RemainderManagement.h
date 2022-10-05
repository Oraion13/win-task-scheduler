#pragma once


/********************************************************************
 This sample schedules a task to start on a daily basis.
********************************************************************/

#define _WIN32_DCOM

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <comdef.h>
#include <wincred.h>
//  Include the task header file.
#include <taskschd.h>
#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsupp.lib")
#pragma comment(lib, "credui.lib")

using namespace std;
class RemainderManagement
{
public:
	int addRemainder(string summary, string description, string date, string time, int recur);
};
