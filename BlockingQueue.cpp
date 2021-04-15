#include<iostream>
#include<queue>
#include<mutex>
#include <windows.h>

using namespace std;

template <typename T>
class BlockingQueue {
private:
	queue<T> _m_queue;
	mutex _m_mtx;
	condition_variable _m_cv;
public:

	BlockingQueue():_m_queue(), _m_mtx(), _m_cv(){

	}

	BlockingQueue(const BlockingQueue& bq) = delete;

	void BqPush(T&& value) {
		unique_lock<mutex> lock(_m_mtx);
		_m_queue.push(move(value));
		_m_cv.notify_all();
	}

	void BqPush(T& value) {
		unique_lock<mutex> lock(_m_mtx);
		_m_queue.push(value);
		_m_cv.notify_all();
	}

	T BqPop() {
		unique_lock<mutex> lock(_m_mtx);

		while (_m_queue.empty()) {
			_m_cv.wait(lock);
		}

		T value(move(_m_queue.front()));
		_m_queue.pop();

		return value;
	}

	size_t BqSize() const
	{
		unique_lock<mutex> lock(_m_mtx);
		return _m_queue.size();
	}
};

int task = 100;
BlockingQueue<int> bq;
mutex mutex_cout;

void worker()
{
	int value;
	thread::id this_id = this_thread::get_id();
	while (true)
	{
		value = bq.BqPop();
		uint64_t sum = 0;
		for (int i = 0; i < value; i++)
		{
			sum += i;
		}

		// 模拟耗时操作
		Sleep(100);

		std::lock_guard<mutex> lock(mutex_cout);
		std::cout << "workder: " << this_id << " "
			<< __FUNCTION__
			<< " line: " << __LINE__
			<< " sum: " << sum
			<< std::endl;
	}
}

void master()
{
	srand(time(nullptr));
	for (int i = 0; i < task; i++)
	{
		bq.BqPush(rand() % 10000);
		cout << "__FUNCTION__" << __LINE__ << i << endl;
		Sleep(20);
	}
}

int main() {

	thread th_master(master);

	vector<thread> threads;
	for (int i = 0; i < 5; i++) {
		//由于这里直接想push_back临时对象，为了提高效率，使用了移动语义，用emplace_back完成，C++11
		threads.emplace_back(thread(worker));
	}

	th_master.join();
	return 0;
}
