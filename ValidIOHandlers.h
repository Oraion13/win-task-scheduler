#pragma once
#include <iostream>
#include <string>

using namespace std;

class ValidIOHandlers
{
public:
	ValidIOHandlers() = default;

	int getInt(string message);
	string getHourMinute();
	string getDate();
	bool isY(string message);
	string getString(string message);

private:
	const int MAX_VALID_YR = 9999;
	const int MIN_VALID_YR = 2022;

	bool isLeap(int year);
	bool isValidDate(int d, int m, int y);
	string doubleDigit(int num);
};

