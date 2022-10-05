#include "ValidIOHandlers.h"

int ValidIOHandlers::getInt(string message) {
	int input = -1;
	bool valid = false;
	do
	{
		cout << message << flush;
		cin >> input;
		if (cin.good())
		{
			//everything went well, we'll get out of the loop and return the value
			valid = true;
		}
		else
		{
			//something went wrong, we reset the buffer's state to good
			cin.clear();
			//and empty it
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input; please re-enter." << endl;
		}
	} while (!valid);

	return (input);
	
}

string ValidIOHandlers::getHourMinute() {
	int hour{};
	int minute{};

	while (true) {
		hour = getInt("Enter a hour [HH](0 - 23): ");

		if (!(hour >= 0 && hour < 24)) {
			cout << "Enter a valid input (Hour) [Number (0 - 23)]!" << endl;

			continue;
		}

		break;
	}

	while (true) {
		minute = getInt("Enter a minute [MM](0 - 59): ");

		if (!(minute >= 0 && minute < 60)) {
			cout << "Enter a valid input (Minute) [Number (0 - 59)]!" << endl;

			continue;
		}
		
		break;
	}
	return doubleDigit(hour) + ":" + doubleDigit(minute);
		
}


// Returns true if
// given year is valid.
bool ValidIOHandlers::isLeap(int year)
{
	// Return true if year
	// is a multiple of 4 and
	// not multiple of 100.
	// OR year is multiple of 400.
	return (((year % 4 == 0) &&
		(year % 100 != 0)) ||
		(year % 400 == 0));
}

// Returns true if given
// year is valid or not.
bool ValidIOHandlers::isValidDate(int d, int m, int y)
{
	// If year, month and day
	// are not in given range
	if (y > MAX_VALID_YR ||
		y < MIN_VALID_YR)
		return false;
	if (m < 1 || m > 12)
		return false;
	if (d < 1 || d > 31)
		return false;

	// Handle February month
	// with leap year
	if (m == 2)
	{
		if (isLeap(y))
			return (d <= 29);
		else
			return (d <= 28);
	}

	// Months of April, June,
	// Sept and Nov must have
	// number of days less than
	// or equal to 30.
	if (m == 4 || m == 6 ||
		m == 9 || m == 11)
		return (d <= 30);

	return true;
}

string ValidIOHandlers::doubleDigit(int num) {
	return num < 10 ? "0" + to_string(num) : to_string(num);
}


string ValidIOHandlers::getDate() {
	cout << "Enter a date: " << endl;
	while (true) {
		int d{}, m{}, y{};
		y = getInt("Enter a year [YYYY]: ");
		m = getInt("Enter a month [MM]: ");
		d = getInt("Enter a day [DD]: ");


		if (!isValidDate(d, m, y)) {
			cout << "Enter a valid date!" << endl;
			continue;
		}

		return to_string(y) + "-" + doubleDigit(m) + "-" + doubleDigit(d);
	}
}

bool ValidIOHandlers::isY(string message) {
	char choice{};

	while (true) {
		cout << message;
		cin >> choice;

		if (tolower(choice) != 'y' && tolower(choice) != 'n') {
			cout << "Enter a valid choice [Y/n]!" << endl;
			continue;
		}

		if (tolower(choice) == 'y') {
			return true;
		}

		return false;
	}
}


string ValidIOHandlers::getString(string message) {
	while (true) {
		string text{};
		cout << message << flush;
		getline(cin, text);

		if (text.size() <= 0) {
			cout << "Enter a valid text!" << endl;
			continue;
		}

		return text;
	}
}

string ValidIOHandlers::getPeriodTime(string message) {
	int hour{};
	int minute{};

	while (true) {
		cout << message << endl;
		hour = getInt("Enter a hour [HH](0 - 23): ");

		if (!(hour >= 0 && hour < 24)) {
			cout << "Enter a valid input (Hour) [Number (0 - 23)]!" << endl;

			continue;
		}

		break;
	}

	while (true) {
		minute = getInt("Enter a minute [MM](0 - 59): ");

		if (!(minute >= 0 && minute < 60)) {
			cout << "Enter a valid input (Minute) [Number (0 - 59)]!" << endl;

			continue;
		}

		break;
	}

	string pt = "PT";
	if (hour != 0) {
		pt.append(to_string(hour));
		pt.append("H");
	}

	pt.append(to_string(minute));
	pt.append("M");

	return pt;
}