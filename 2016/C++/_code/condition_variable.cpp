#include <iostream>
#include <condition_variable>
#include <mutex>
#include <thread>

std::mutex _mutex;
std::condition_variable condVar;

bool dataReady;

void doTheWork()
{
	std::cout << "Processing shared data.\n";
}

void waitingForWork()
{
	std::cout << "Work: Waiting for work.\n";

	std::unique_ptr<std::mutex> lk(_mutex);
	condVar.wait(lk, [] { return dataReady; });
	doTheWork();
	std::cout << "Work done.\n";
}

void setDataReady()
{
	std::lock_guard<std::mutex> lk(_mutex);
	dataReady = true;
	std::cout << "Sender: Data is ready.\n";
	condVar.notify_one();
}

void test_CV()
{
	std::thread t1(waitingForWork);
	std::thread t2(setDataReady);

	t1.join();
	t2.join();
}