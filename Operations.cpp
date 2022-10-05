#include "Operations.h"

void Operations::displayOperations() {
	cout << endl;
	cout << "---------------------------" << endl;
	cout << "-------- Remainder --------" << endl;
	cout << "---------------------------" << endl;

	cout << "0 - Exit" << endl;
	cout << "1 - Create Remainders" << endl;
	cout << "2 - Read Remainders" << endl;
	//cout << "3 - Update Remainders" << endl;
	cout << "3 - Delete Remainders" << endl;
}

void Operations::operations() {
	while (true) {
		displayOperations();

		cout << "Enter '0' to exit..." << endl;
		int choice = validIOHandlers->getInt("Enter a choice [Number / 0]: ");

		if (choice == 0) {
			break;
		}

		switch (choice) {
		case 1: {
			remainder->createEvent();
		}
			  break;
			  
		case 2: {
			remainder->readEvent();
		}
			  break;
		//case 3: {
		//	remainder->updateEvent();
		//}
		//	  break;
		case 3: {
			remainder->deleteEvent();
		}
			  break;

		default:
			break;
		}
	}
}