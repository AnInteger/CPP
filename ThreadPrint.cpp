#include <iostream>
#include <thread>
#include <mutex>
#include <Windows.h>
using namespace std;

mutex mtx;
condition_variable cv;
bool flag = true;


void func1() {

	while (1) {
		unique_lock<mutex> lock(mtx);
		cv.wait(lock, [] { return !flag; });
		cout << "Thread 1" << endl;
		flag = true;
		cv.notify_one();
		Sleep(100);
	}

}

void func2() {
	while (1) {
		unique_lock<mutex> lock(mtx);
		cv.wait(lock, [] { return flag; });
		cout << "Thread 2" << endl;
		flag = false;
		cv.notify_one();
		Sleep(100);
	}
}

int main() {
	thread t1(func1);
	thread t2(func2);

	t1.join();
	t2.join();
	return 0;
}
