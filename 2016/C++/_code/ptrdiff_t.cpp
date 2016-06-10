#include <iostream>

int main()
{
	int a = -2;
	unsigned b = 1;

	//size_t a = -2;
	//size_t b = 1;
	// or
	//ptrdiff_t a = -2;
	//ptrdiff_t b = 1;

	int ary[5] = { 1,2,3,4,5 };

	int *ptr = ary + 3;

	// 在32位平台上，因为 a + b = 0xFFFFFFFF 也就是-1，所以最终ptr=ptr-1，结果为3
	// 但是在64位平台上，则会是个错误的值。
	// 所以上述a和b应该使用 size_t or ptrdiff_t类型
	ptr = ptr + (a + b);
	printf("%i\n", *ptr);

	return 1;
}