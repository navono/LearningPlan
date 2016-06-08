#include <iostream>
#include <functional>

int testFunc(int a, char c, float f)
{
	std::cout << a << std::endl;
	std::cout << c << std::endl;
	std::cout << f << std::endl;

	return 1;
}

int main()
{
	auto bindFunc1 = std::bind(testFunc, std::placeholders::_1, 'A', 1.09);
	bindFunc1(10);

	std::cout << "=================================\n";

	auto bindFunc2 = bind(testFunc, std::placeholders::_2, std::placeholders::_1, 100.1);
	bindFunc2('B', 10);

	std::cout << "=================================\n";

	auto bindFunc3 = bind(testFunc, std::placeholders::_2, std::placeholders::_3, std::placeholders::_1);
	bindFunc3(100.1, 30, 'C');

	// 在bind的时候，第一个位置是TestFunc，除了这个，参数的第一个位置为占位符std::placeholders::_2，
	// 这就表示，调用bindFunc3的时候，它的第二个参数和TestFunc的第一个参数匹配，以此类推

	//以下是使用std::bind的一些需要注意的地方：

	//	bind预先绑定的参数需要传具体的变量或值进去，对于预先绑定的参数，是pass - by - value的；
	//	对于不事先绑定的参数，需要传std::placeholders进去，从_1开始，依次递增。placeholder是pass - by - reference的；
	//	bind的返回值是可调用实体，可以直接赋给std::function对象；
	//	对于绑定的指针、引用类型的参数，使用者需要保证在可调用实体调用之前，这些参数是可用的；
	//	类的this可以通过对象或者指针来绑定。

	
	// bind函数组合
	// 找出集合中大于5，小于10的元素个数
	
	// 首先一个逻辑与
	// std::bind(std::logical_and<bool>(), _1, _2);
	
	// _1是否大于5的谓词
	// std::bind(std::greater<int>(), _1, 5);
	
	// _2是否小于10的谓词
	// std::bind(std::less_equal<int>(), _1, 10);
	
	// 组合起来
	auto f = std::bind(std::logical_and<bool>(), 
					   std::bind(std::greater<int>(), _1, 5)),
					   std::bind(std::less_equal<int>(), _1, 10));
	auto count = std::count_if(coll.begin(), coll.end(), f);
	
	return 0;
}