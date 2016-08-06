#include <atomic>
#include <thread>
#include <iostream>

class Spinlock
{
	std::atomic_flag flag = ATOMIC_FLAG_INIT;

public:
	Spinlock() {}

	void lock()
	{
		while (flag.test_and_set(std::memory_order_acquire))
			;
	}

	void unlock()
	{
		flag.clear(std::memory_order_release);
	}
};

Spinlock spin;

void workOnResouce(std::string param)
{
	spin.lock();
	std::cout << param.c_str() << "	work on shared resource\n";
	spin.unlock();
}

int main()
{
	std::thread t1(workOnResouce, "t1");
	std::thread t2(workOnResouce, "t2");

	t1.join();
	t2.join();

	return 1;
}