#include <condition_variable>
#include <iostream>
#include <thread>
#include <vector>

std::vector<int> mySharedWork;
std::mutex mut;
std::condition_variable condVar;

bool dataReady;

void waitingForWork()
{
	std::unique_lock<std::mutex> lk(mut);
	std::cout << "Waiting " << std::endl;

	condVar.wait(lk, [] { return dataReady; });
	mySharedWork[1] = 2;

	std::cout << "Work done " << std::endl;
}

void setDataReady()
{
	mySharedWork = { 1,0,3 };
	{
		std::lock_guard<std::mutex> lk(mut);
		dataReady = true;
	}

	std::cout << "Data prepared " << std::endl;
	condVar.notify_one();
}

int main()
{
	std::cout << std::endl;
	
	std::thread t1(waitingForWork);
	std::thread t2(setDataReady);

	t1.join();
	t2.join();

	for (auto v : mySharedWork)
	{
		std::cout << v << " ";
	}

	std::cout << std::endl;

	return 1;
}