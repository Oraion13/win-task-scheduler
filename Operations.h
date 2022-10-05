#pragma once
#include <iostream>

#include "ValidIOHandlers.h"
#include "Remainder.h"

using namespace std;

class Operations
{
public:
	Operations() {
		validIOHandlers = new ValidIOHandlers();
		remainder = new Remainder();
		operations();
	}

	void displayOperations();
	void operations();

private:
	ValidIOHandlers* validIOHandlers;
	Remainder* remainder;
};

