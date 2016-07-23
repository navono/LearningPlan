#include <mutex>

class Singleton
{
private:
	static std::once_flag initFlag;
	static Singleton* instance;

	Singleton() = default;
	~Singleton() = default;

public:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	static Singleton* getInstance()
	{
		std::call_once(initFlag, Singleton::initSingletion);
		return instance;
	}

	static void initSingletion()
	{
		instance = new Singleton();
	}
};

Singleton *Singleton::instance = nullptr;
std::once_flag Singleton::initFlag;



//////////////////////////////////////////////////////////////////////////

class MySingleton {
public:
	static MySingleton& getInstance() {
		static MySingleton instance;
		return instance;
	}
private:
	MySingleton();
	~MySingleton();
	MySingleton(const MySingleton&) = delete;
	MySingleton& operator=(const MySingleton&) = delete;

};

MySingleton::MySingleton() = default;
MySingleton::~MySingleton() = default;