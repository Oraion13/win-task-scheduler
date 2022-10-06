#pragma once
#include <iostream>

#include "ValidIOHandlers.h"
#include "Remainder.h"
#include "CreateTask.h"

using namespace std;

class Operations
{
public:
	Operations() {
		validIOHandlers = new ValidIOHandlers();
		remainder = new Remainder();
		createEvent = new CreateTask();
		operations();
	}

	void displayOperations();
	void operations();

private:
	ValidIOHandlers* validIOHandlers;
	Remainder* remainder;
	CreateTask* createEvent;
};

