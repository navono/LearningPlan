#include <atomic>
#include <thread>
#include <iostream>
#include <string>

std::atomic<std::string*> ptr;
int data;
std::atomic<int> atomicData;


void producer()
{
	std::string *p = new std::string("C++ 11");
	data = 2016;
	atomicData.store(2046, std::memory_order_relaxed);
	ptr.store(p, std::memory_order_release);
}

void consumer()
{
	std::string *p2;
	while (!(p2 = ptr.load(std::memory_order_consume)))
		;
	
	std::cout << "*p2: " << *p2 << std::endl;
	std::cout << "data: " << data << std::endl;
	std::cout << "atomicData: " << atomicData.load(std::memory_order_relaxed) << std::endl;
}

int main()
{
	std::cout << std::endl;

	std::thread t1(producer);
	std::thread t2(consumer);

	t1.join();
	t2.join();

	return 1;
}