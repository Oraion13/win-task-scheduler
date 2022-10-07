#pragma once
#include <iostream>

#include "ValidIOHandlers.h"
#include "Remainder.h"
#include "CreateTask.h"
#include "UpdateTask.h"

using namespace std;

class Operations
{
public:
	Operations() {
		validIOHandlers = new ValidIOHandlers();
		remainder = new Remainder();
		createTask = new CreateTask();
		updateTask = new UpdateTask();
		operations();
	}

	void displayOperations();
	void operations();

private:
	ValidIOHandlers* validIOHandlers;
	Remainder* remainder;
	CreateTask* createTask;
	UpdateTask* updateTask;
};

