#include <windows.h>
#include <iostream>
#include <conio.h>
#include <string>
#include <thread>
#include <stdlib.h>
#include <vector>
#include <assert.h>
#include <chrono>
#include <future>
#include <gdiplus.h>
#include <ctime>
#include <wininet.h>
#include <winuser.h>
#include <conio.h>
#include <time.h>
#include <fstream>
#include <strsafe.h>
#include <io.h>
#include <crtdefs.h>
#include <GdiPlus.h>
#include <lmcons.h>

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

#define PATH_TO_TIME "Z:/OS/time.txt"
#define PATH_TO_PASSWORD "Z:/OS/parentAndChildPassword.txt"
#define PATH_TO_TIME_USED "Z:/OS/time_used.txt"
#define PATH_TO_HISTORY "Z:/computer_history"

using namespace std;
//char acUserName[100];

using namespace Gdiplus;

fstream err("errormul.txt", ios::app);
fstream log_error_file("log_error.txt", ios::app);

#pragma comment(lib, "user32.lib") 
#pragma comment(lib,"Wininet.lib")
#pragma comment (lib,"gdiplus.lib")

void screenshot() {
	SYSTEMTIME st;  // create object of system time 
	GetLocalTime(&st);
	int year = st.wYear;  // extract year from system time
	int month = st.wMonth; // extract month from system time
	int day = st.wDay; // extract year day system time
	int hour = st.wHour; // extract year hours system time
	int mintue = st.wMinute; // extract mintue from system time

	string yearS = to_string(year);
	yearS += "_";
	string monthS = to_string(month);
	monthS += "-";
	string dayS = to_string(day);
	dayS += "-";
	string hourS = to_string(hour);
	hourS += "H-";
	string mintueS = to_string(mintue);
	mintueS += "M.jpg";

	string file;
	file = PATH_TO_HISTORY + dayS + monthS + yearS + hourS + mintueS;  // create complete string of date and time


	ULONG_PTR gdiplustoken;
	GdiplusStartupInput gdistartupinput;
	GdiplusStartupOutput gdistartupoutput;

	gdistartupinput.SuppressBackgroundThread = true;
	GdiplusStartup(&gdiplustoken, &gdistartupinput, &gdistartupoutput); //start GDI+

	HDC dc = GetDC(GetDesktopWindow());//get desktop content
	HDC dc2 = CreateCompatibleDC(dc);    //copy context

	RECT rc0kno;  // rectangle  Object

	GetClientRect(GetDesktopWindow(), &rc0kno);// get desktop size;
	int w = rc0kno.right - rc0kno.left;//width
	int h = rc0kno.bottom - rc0kno.top;//height

	HBITMAP hbitmap = CreateCompatibleBitmap(dc, w, h);  //create bitmap
	HBITMAP holdbitmap = (HBITMAP)SelectObject(dc2, hbitmap);

	BitBlt(dc2, 0, 0, w, h, dc, 0, 0, SRCCOPY);  //copy pixel from pulpit to bitmap
	Bitmap* bm = new Bitmap(hbitmap, NULL);

	UINT num;
	UINT size;

	ImageCodecInfo* imagecodecinfo;
	GetImageEncodersSize(&num, &size); //get count of codec

	imagecodecinfo = (ImageCodecInfo*)(malloc(size));
	GetImageEncoders(num, size, imagecodecinfo);//get codec

	CLSID clsidEncoder;

	for (int i = 0; i < num; i++)
	{
		if (wcscmp(imagecodecinfo[i].MimeType, L"image/jpeg") == 0)
			clsidEncoder = imagecodecinfo[i].Clsid;   //get jpeg codec id

	}

	free(imagecodecinfo);

	wstring ws;
	ws.assign(file.begin(), file.end());  //sring to wstring
	bm->Save(ws.c_str(), &clsidEncoder);   //save in jpeg format
	SelectObject(dc2, holdbitmap);  //Release Objects
	DeleteObject(dc2);
	DeleteObject(hbitmap);

	ReleaseDC(GetDesktopWindow(), dc);
	GdiplusShutdown(gdiplustoken);
}

/*int hours= 0;
int minutes = 0;
int seconds = 0;*/
int year, month, day, phour, pmin;

class TimeFrame {
public:
	vector<int> F; // mang bat dau, gom 2 phan tu, F[0]: gio, F[1]: phut
	vector<int> T; // mang ket thuc, gom 2 phan tu, T[0]: gio, T[1]: phut
	int D, I, S, F_by_min, T_by_min; // cac thong so tuong ung trong de bai
	int U=0; //time_use by min
	TimeFrame();
	TimeFrame(vector<int> F, vector<int> T, int D, int I, int S, int F_by_min, int T_by_min) {
		this->F = F;
		this->T = T;
		this->D = D;
		this->I = I;
		this->S = S;
		this->F_by_min = F_by_min;
		this->T_by_min = T_by_min;
	}
	bool operator==(TimeFrame& obj) {
		if (this->F == obj.F &&
			this->T == obj.T &&
			this->D == obj.D &&
			this->I == obj.I &&
			this->S == obj.S) return true;
		return false;
	}
	bool operator!=(TimeFrame& obj) {
		return !(*this == obj);
	}
	vector<int> getF() { return F; };
	vector<int> getT() { return T; };
	int getD() { return D; };
	int getI() { return I; };
	int getS() { return S; };
};

vector<string> readParentsAndChildPassword() {
	ifstream filein;
	filein.open(PATH_TO_PASSWORD, ios_base::in);
	string pp, cp;
	vector<string> p;
	getline(filein, pp);
	p.push_back(pp);
	getline(filein, cp);
	p.push_back(cp);
	filein.close();
	return p;
}

void readTextFile(vector<TimeFrame>& tf) {
	ifstream filein;
	filein.open(PATH_TO_TIME, ios_base::in);
	while (!filein.eof()) {
		vector<int> F, T;
		int D = 0, I = 0, S = 0;
		int F_by_min, T_by_min;
		string str = "";
		getline(filein, str);
		string tmp = "";
		tmp = str[1];
		tmp += str[2];
		F.push_back(stoi(tmp));
		tmp = str[4];
		tmp += str[5];
		F.push_back(stoi(tmp));
		F_by_min = F[0] * 60 + F[1];

		tmp = str[8];
		tmp += str[9];
		T.push_back(stoi(tmp));
		tmp = str[11];
		tmp += str[12];
		T.push_back(stoi(tmp));
		T_by_min = T[0] * 60 + T[1];


		for (int i = 13; i < str.size(); i++) {
			if (str[i] == 'S') {
				string tmp = "";
				int j = i + 1;

				for (; j < str.size() && str[j] != ' '; j++) {
					tmp += str[j];
				}
				S = stoi(tmp);
				i = j;
			}
			else if (str[i] == 'I') {
				string tmp = "";
				int j = i + 1;
				for (; j < str.size() && str[j] != ' '; j++) {
					tmp += str[j];
				}
				I = stoi(tmp);
				i = j;
			}
			else if (str[i] == 'D') {
				string tmp = "";
				int j = i + 1;
				for (; j < str.size() && str[j] != ' '; j++) {
					tmp += str[j];
				}
				D = stoi(tmp);
				i = j;
			}
		}
		TimeFrame temp(F, T, D, I, S, F_by_min, T_by_min);
		tf.push_back(temp);
	}
	filein.close();
}


void dem15(future<void> futureObj) {
	int seconds = 0;
	while (futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
	{
		cout << ++seconds << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		if (seconds == 5) { // dem 15s
			// shutdown
			//system("shutdown -s -t 0");
			cout << "shutdown";
			return;
		}
	}
}

void nhapmatkhau(string pp, string& flag) {
	string pw;
	while (1) {
		cout << "Input password: " << endl; cin >> pw;
		if (pw == pp) {
			flag = "1";
			return;
		}
	}
}

bool change_time(vector<TimeFrame> tf1, vector<TimeFrame> tf2) {
	for (int i = 0; i < tf1.size(); i++) {
		if (tf1[i] != tf2[i]) return true;
	}
	return false;
}

vector<int> min_to_hour(int min) {
	vector<int> time;
	int h = min / 60;
	int m = min % 60;
	time.push_back(h);
	time.push_back(m);
	return time;
}

void Notify(vector<TimeFrame> tf, int start, int k) {
	time_t t = time(0);   // get time now
	tm* now = new tm;
	localtime_s(now, &t);
	int phour = now->tm_hour, pmin = now->tm_min;
	int time_now = phour * 60 + pmin;

	int time_used = time_now - start;

	int index_tf_now = -1;
	for (int i = 0; i < tf.size(); i++) {
		if (tf[i].F_by_min <= start && start <= tf[i].T_by_min) index_tf_now = i;
	}

	// time_used < tf[i].D
	// time_used + tf[i].U < tf[i].S
	// time_now < tf[i].T_by_min

	//tat Start + D, mo Start + D + I
	//tat F+S, mo tf[i+1].F
	//tat T, mo tf[i+1].F
	int on, off;
	if (tf[index_tf_now].D != 0) {
		off = start + tf[index_tf_now].D;
		on = off + tf[index_tf_now].I;
		if (tf[index_tf_now].S != 0) {
			if (tf[index_tf_now].D + tf[index_tf_now].U > tf[index_tf_now].S) {
				off = start + tf[index_tf_now].S - tf[index_tf_now].U;
				if (index_tf_now + 1 < tf.size()) on = tf[index_tf_now + 1].F_by_min;
				else on = tf[0].F_by_min;
			}
		}
		if (off > tf[index_tf_now].T_by_min) off = tf[index_tf_now].T_by_min;
		if (on >= tf[index_tf_now].T_by_min) {
			if (index_tf_now + 1 < tf.size()) on = tf[index_tf_now + 1].F_by_min;
			else on = tf[0].F_by_min;
		}
	}
	else {
		if (tf[index_tf_now].S != 0) {
			off = start + tf[index_tf_now].S - tf[index_tf_now].U;
			if (index_tf_now + 1 < tf.size()) on = tf[index_tf_now + 1].F_by_min;
			else on = tf[0].F_by_min;
		}
		else {
			off = tf[index_tf_now].T_by_min;
			if (index_tf_now + 1 < tf.size()) on = tf[index_tf_now + 1].F_by_min;
			else on = tf[0].F_by_min;
		}
	}
	if (k == 3) {
		if (off - time_now == 1) cout << "Con 1 phut nua tat may" << endl;
		if (off - time_now == 0) { 
			cout << "shut down" << endl; 
			// shut down
			//system("shutdown -s -t 0");
		}
	}
	if (k == 2) {
		vector<int> time_off = min_to_hour(off);
		cout << "Thoi gian tat may: " << time_off[0] << ":" << time_off[1] << endl;
		vector<int> time_on = min_to_hour(on);
		cout << "Thoi gian mo may: " << time_on[0] << ":" << time_on[1] << endl;
	}
}

int main()
{
	vector<string> respw = readParentsAndChildPassword();  // read parents' and childs' password
	string parentsPassword = respw[0];
	string childsPassword = respw[1];
	vector<TimeFrame> tf;
	readTextFile(tf);
	time_t t = time(0);   // get time now
	tm* now = new tm;
	localtime_s(now, &t);
	phour = now->tm_hour; pmin = now->tm_min;
	int start = phour * 60 + pmin;
	int index_tf_now;
	for (int i = 0; i < tf.size(); i++) {
		if (tf[i].F_by_min <= start <= tf[i].T_by_min) index_tf_now = i;
	}
	string pw;
	int c = 0;
	while (1) {
		cout << "Nhap mat khau: "; cin >> pw;
		if (pw == parentsPassword) {
			// doi 60' roi hoi mk lai
			// Sleep(5000); // 5s cho nhanh
			/*while (c != 5) {
				cout << ++c << endl;
				Sleep(1000);
			}*/
			Sleep(1000 * 60 * 60);
			continue;
		}
		else {
			break;
		}
	}

	// kiem tra dc dung chua
	bool ok = false;
	int i = 0;
	for (; i < tf.size(); i++) {
		if (phour >= tf[i].getF()[0] && phour < tf[i].getT()[0]) {
			ok = true;
			break;
		}
		if (phour == tf[i].getF()[0] && phour == tf[i].getT()[0] && pmin >= tf[i].getF()[1] && pmin <= tf[i].getT()[1]) {
			ok = true;
			break;
		}
	}
	if (!ok) { // chua dc dung
		for (int i = 0; i < tf.size(); i++) {
			cout << "Khung gio dung may la: " << tf[i].getF()[0] << ":" << tf[i].getF()[1] << "->" << tf[i].getT()[0] << ":" << tf[i].getT()[1] << endl;
		}
		// kiem tra dem 15s & nhap dung mk phu huynh

		while (1) {
			promise<void> exitSignal;
			future<void> futureObj = exitSignal.get_future();
			string flag = "0";

			thread dem(&dem15, move(futureObj));
			thread NhapMatKhau(nhapmatkhau, parentsPassword, ref(flag));
			NhapMatKhau.join();
			if (flag == "1") {
				exitSignal.set_value();  // huy thread dem 15s kia di, de khoi bi tat may
				dem.join();
				int counter = 0;
				while (1) { // doi 60' xong hoi lai
					counter++;
					//cout << counter << endl;
					Sleep(1000);
					if (counter == 5) { // doi 5s cho nhanh
						break;
					}
				}
				// doi xong thi chay lai vong while, tao lai thread dem va nhapmk
			}
		}

	}
	else { // duoc dung may
		// kiem tra mk tre
		int i = 1;
		string str;
		int cnt = 1;
		while (i <= 4) {
			if (i == 4) {
				// nhap sai 3 lan, dat thoi gian ko dc dung may la 10' roi tat may
				Sleep(5000); // 5s cho nhanh
			   // shut down
			   //system("shutdown -s -t 0");
			   /*while (cnt != 6) {
				   Sleep(1000);
				   cout << cnt << endl;
				   cnt++;
			   }*/
				cout << "shut down";
				break;
			}
			cout << "Nhap mat khau (khong phai phu huynh): ";
			cin >> str;
			if (str != childsPassword) {
				i++;
				continue;
			}
			else { // dung mat khau roi
				// chup man hinh moi 60s
				// doc thong tin khung gio duoc dung
				//          -> thay khung gio bi thay doi
				//          -> thong bao thoi gian tat may va bat may lai
				// doc thong tin khung gio duoc dung
				//          -> thay con 1 phut nua thi may se tat
				//          -> thong bao thoi gian tat may va bat may lai
				//          -> thay con 0 phut thi may se tat
				//          -> tat may

				vector<TimeFrame> tf2;
				int index_tf_now;
				for (int i = 0; i < tf.size(); i++) {
					if (tf[i].F_by_min <= start && start <= tf[i].T_by_min) index_tf_now = i;
					else tf[i].U = 0;
				}

				int t = start;
				while (true) {
					time_t now = time(0);
					tm* ltm = localtime(&now);
					int h = ltm->tm_hour, m = ltm->tm_min;
					int now_by_min = h * 60 + m;

					tf[index_tf_now].U = now_by_min - start;

					if (now_by_min - t == 1) {
						ofstream fo;
						fo.open(PATH_TO_TIME_USED);
						for (int i = 0; i < tf.size(); i++) {
							fo << tf[i].U << endl;
						}
						fo.close();

						cout << "Cap screen" << endl;
						screenshot();

						Notify(tf, start, 3);
						t = now_by_min;
					}

					readTextFile(tf2);
					if (tf.size() != tf2.size()) {
						cout << "changed..." << endl;
						tf = tf2;
						Notify(tf, start, 2);
					}
					else {
						if (change_time(tf, tf2)) {
							cout << "changed..." << endl;
							tf = tf2;
							Notify(tf, start, 2);
						}
					}
					tf2.clear();
				}
				}
			}
		}
	return 0;
}