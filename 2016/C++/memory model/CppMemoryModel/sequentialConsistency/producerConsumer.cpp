#include <atomic>
#include <iostream>
#include <string>
#include <thread>


/// Heavyweight synchronization

std::string work;
std::atomic<bool> ready(false);

void consumer()
{
	while (!ready.load())
	{}

	std::cout << work << std::endl;
}

void producer()
{
	work = "done";
	ready = true;
}

int main()
{
	std::thread t1(producer);
	std::thread t2(consumer);
	
	t1.join();
	t2.join();

	return 1;
}


//以上代码的解释是：
//
//= > Means it follows in the next lines :
//
//1. work = "done"  is sequenced - before ready = true = > work = "done" happens-before ready = true
//2. while (!ready.load()) {} is sequenced-before std::cout << work << std::endl
//= > while (!ready.load()) {} happens-before std::cout << work << std::endl
//3. ready = true synchronizes-with while (!ready.load()) {} = > ready = true inter-thread happens-before while (!ready.load()) {}
//= > ready = true happens-before while (!ready.load()) {}
//
//= > Because the happens-before relation is transitive, it follows t : work = "done" happens - before
//ready = true happens-before while (!ready.load()) {} happens-before std::cout << work << std::endl
