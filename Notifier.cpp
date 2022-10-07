#include <Windows.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
	// Hide the shell
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	char title[300] = "";
	char desc[300] = "";

	int index = 1;
	// extract heading, if the pattern '0809x' hits in the arguments then break
	cout << "argc: " << argc << endl;
	for (index; index < argc; index++) {
		cout << argv[index] << endl;
		if (strcmp(argv[index], "0809x") == 0) {
			cout << "breaked" << endl;
			++index;
			break;
		}

		strcat_s(desc, sizeof(desc), argv[index]);
		strcat_s(desc, sizeof(desc), " ");
	}

	// extract description
	for (index; index < argc; index++) {
		cout << argv[index] << endl;
		strcat_s(title, sizeof(title), argv[index]);
		strcat_s(title, sizeof(title), " ");
	}

	if (MessageBoxA(0, (LPCSTR)title, (LPCSTR)desc, MB_OK | MB_ICONINFORMATION)) {
		cout << "success" << endl;
	}
	else {
		cout << "failed" << endl;
		cout << to_string(GetLastError()) << endl;
	}

	return 0;
}