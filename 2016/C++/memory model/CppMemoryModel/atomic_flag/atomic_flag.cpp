#include <atomic>
#include <thread>
#include <iostream>
#include <chrono>

class Spinlock
{
	std::atomic_flag flag = ATOMIC_FLAG_INIT;

public:
	Spinlock()
	{}

	void lock()
	{
		while (flag.test_and_set())
		{
			;
		}
	}

	void unlock()
	{
		flag.clear();
	}
};

Spinlock spin;

void workOnResource()
{
	spin.lock();
	// shared resource
	
	std::cout << "workOnResource on:  " << std::this_thread::get_id() 
		<< " sleep 5 sec" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(5));

	spin.unlock();
}

int main()
{
	std::thread t1(workOnResource);
	std::thread t2(workOnResource);

	t1.join();
	t2.join();

	return 1;
}


// mutex version
//#include <mutex>
//#include <thread>
//#include <chrono>
//#include <iostream>
//
//std::mutex mut;
//
//void workOnResource()
//{
//	std::lock_guard<std::mutex> lg(mut);
//	std::this_thread::sleep_for(std::chrono::microseconds(5000));
//	std::cout << "workOnResource\n";
//}
//
//int main()
//{
//	std::thread t1(workOnResource);
//	std::thread t2(workOnResource);
//
//	t1.join();
//	t2.join();
//
//	return 1;
//}