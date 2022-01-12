#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <vector>
using namespace std;

#define TIME "Z:/OS/time.txt"
#define TIME_USED "Z:/OS/time_used.txt"
#define PATH_TO_SIGNAL "Z:/OS/semaphore.signal"
#define PATH_TO_TIME "start Z:\\OS\\time.txt"
#define PATH_TO_HISTORY "explorer Z:\\OS\\computer_history"

void adjustTime() {
	ifstream si;
	ofstream so;
	int signal;
	int k = 0;

	do {
		if (k == 1) {
			cout << "Wait! Someone now is editting this file..." << endl;
			int command;
			cout << "1. Still wait" << endl;
			cout << "2. Back to menu" << endl;
			do {
				cout << ">> "; cin >> command;
			} while (command != 1 && command != 2);
			if (command == 2) return;
		}

		si.open(PATH_TO_SIGNAL);
		if (si.peek() == EOF) signal = 1;
		else si >> signal;

		si.close();

		if (k < 2) k++;
	} while (signal <= 0);

	signal--;
	so.open(PATH_TO_SIGNAL);
	so << signal;
	so.close();

	ofstream fo;
	fo.open(TIME);
	vector<string> list_time;
	int n; 
	cout << "N= "; cin >> n;
	for (int i = 0; i < n; i++) {
		string time;
		cin.ignore();
		cout << "Input time: ";
		getline(cin, time);
		list_time.push_back(time);
	}
	for (int i = 0; i < list_time.size(); i++)
		fo << list_time[i] << endl;
	fo.close();

	signal++;
	so.open(PATH_TO_SIGNAL);
	so << signal;
	so.close();

	// doi file quan ly thoi gian => thoi gian su dung se duoc reset
	fo.open(TIME_USED);
	fo.close();
}

void main() {
	while (true) {
		system("cls");
		int command;
		cout << "Menu" << endl;
		cout << "1. Watch managed-time" << endl;
		cout << "2. Change managed-time" << endl;
		cout << "3. Watch child computer-using history" << endl;
		cout << "0. Exit" << endl;
		do {
			cout << ">> "; cin >> command;
		} while (command < 0 && command >3);
		if (command == 1) {
			ifstream fo;
			string time;
			fo.open(TIME);
			getline(fo,time);
			cout << time << endl;
			system("pause");
		}
		if (command == 2) adjustTime();
		if (command == 3) system(PATH_TO_HISTORY);
		if (command == 0) exit(0);
	}

}