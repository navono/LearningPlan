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

	// ��32λƽ̨�ϣ���Ϊ a + b = 0xFFFFFFFF Ҳ����-1����������ptr=ptr-1�����Ϊ3
	// ������64λƽ̨�ϣ�����Ǹ������ֵ��
	// ��������a��bӦ��ʹ�� size_t or ptrdiff_t����
	ptr = ptr + (a + b);
	printf("%i\n", *ptr);

	return 1;
}