#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>


/// The condition variable notifies the waiting thread(condVar.notify()), that it should proceed with its work.
/// But the waiting thread with the atomic boolean checks, if the sender is done with its work(dataRead = true).

std::vector<int> mySharedWork;
std::atomic<bool> dataReady(false);

void waitingForWork()
{
	std::cout << "Waiting " << std::endl;
	while (!dataReady.load())					// 3
	{
		std::this_thread::sleep_for(std::chrono::microseconds(5));
	}
	
	mySharedWork[1] = 2;						// 4
	std::cout << "Work done " << std::endl;
}

void setDataReady()
{
	mySharedWork = { 1,0,3 };					// 1
	dataReady = true;							// 2
	std::cout << "Data prepared " << std::endl;
}

int main()
{
	// 1 happens-before 2
	// 3 happens-before 4
	// 2 synchronizes-with 3
	// happens-before is transitive, so 1 happens-before 4

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