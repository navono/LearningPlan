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

	// ��bind��ʱ�򣬵�һ��λ����TestFunc����������������ĵ�һ��λ��Ϊռλ��std::placeholders::_2��
	// ��ͱ�ʾ������bindFunc3��ʱ�����ĵڶ���������TestFunc�ĵ�һ������ƥ�䣬�Դ�����

	//������ʹ��std::bind��һЩ��Ҫע��ĵط���

	//	bindԤ�Ȱ󶨵Ĳ�����Ҫ������ı�����ֵ��ȥ������Ԥ�Ȱ󶨵Ĳ�������pass - by - value�ģ�
	//	���ڲ����Ȱ󶨵Ĳ�������Ҫ��std::placeholders��ȥ����_1��ʼ�����ε�����placeholder��pass - by - reference�ģ�
	//	bind�ķ���ֵ�ǿɵ���ʵ�壬����ֱ�Ӹ���std::function����
	//	���ڰ󶨵�ָ�롢�������͵Ĳ�����ʹ������Ҫ��֤�ڿɵ���ʵ�����֮ǰ����Щ�����ǿ��õģ�
	//	���this����ͨ���������ָ�����󶨡�

	
	// bind�������
	// �ҳ������д���5��С��10��Ԫ�ظ���
	
	// ����һ���߼���
	// std::bind(std::logical_and<bool>(), _1, _2);
	
	// _1�Ƿ����5��ν��
	// std::bind(std::greater<int>(), _1, 5);
	
	// _2�Ƿ�С��10��ν��
	// std::bind(std::less_equal<int>(), _1, 10);
	
	// �������
	auto f = std::bind(std::logical_and<bool>(), 
					   std::bind(std::greater<int>(), _1, 5)),
					   std::bind(std::less_equal<int>(), _1, 10));
	auto count = std::count_if(coll.begin(), coll.end(), f);
	
	return 0;
}