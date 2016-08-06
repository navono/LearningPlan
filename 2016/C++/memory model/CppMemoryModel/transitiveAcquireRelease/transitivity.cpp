#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

std::vector<int> mySharedWork;
std::atomic<bool> dataProduced{ false };
std::atomic<bool> dataConsumed{ false };

//Thread t2 waits in line 21, until thread t3 has set dataProduced on true (line 16).
//Thread t1 waits in line 28, until thread t2 has set dataConsumed on true (line 23).

void dataProducer()
{
	mySharedWork = { 1,0,3 };
	dataProduced.store(true, std::memory_order_release);
}

void deliveryBoy()
{
	while (!dataProduced.load(std::memory_order_acquire))
		;
	dataConsumed.store(true, std::memory_order_release);
}

void dataConsumner()
{
	while (!dataConsumed.load(std::memory_order_acquire))
		;
	mySharedWork[1] = 2;
}

int main()
{
	std::cout << std::endl;

	std::thread t1(dataConsumner);
	std::thread t2(deliveryBoy);
	std::thread t3(dataProducer);

	t1.join();
	t2.join();
	t3.join();

	for (auto v : mySharedWork)
	{
		std::cout << v << " ";
	}

	std::cout << std::endl;

	return 1;
}