// youtube  -> Bo Qian

#include <regex>
#include <iostream>
#include <string>

using namespace std;

void test_basic(const string& str)
{
	//regex e("abc.", regex_constants::icase);		// .  Any character except newline. 1���ַ�
	//regex e("abc?");						// ?		Zero or 1 preceding character. ƥ��?ǰһ���ַ�
	//regex e("abc*");						// *		Zero or more preceding character. ƥ��*ǰһ���ַ�
	//regex e("abc+");						// +		One or more preceding character. ƥ��+ǰһ���ַ�
	//regex e("ab[cd]*");					// [...]	Any character inside square brackets. ƥ��[]�ڵ������ַ�
	//regex e("ab[^cd]*");					// [...]	Any character not inside square brackets. ƥ���[]�ڵ������ַ�
	//regex e("ab[cd]{3}");					// {n}		ƥ��{}֮ǰ�����ַ������ַ�����Ϊ3��
	//regex e("ab[cd]{3,}");				// {n}		ƥ��{}֮ǰ�����ַ������ַ�����Ϊ3����3������
	//regex e("ab[cd]{3,5}");				// {n}		ƥ��{}֮ǰ�����ַ������ַ�����Ϊ3�����ϣ�5�����±�����
	//regex e("abc|de[fg]");				// |		ƥ��|���ߵ�����һ������
	//regex e("(abc)de+\\1");				// ()       ()��ʾһ���ӷ��飬��\1��ʾ�ڴ�λ��ƥ���һ�����������
	//regex e("(abc)c(de+)\\2\\1");			// \2		��ʾ�����ڴ�ƥ��ڶ������������
	//regex e("[[:w:]]+@[[:w:]]+\.com");	//	[[:w:]]	word character: digit, number, underscore

	//bool match = regex_match(str, e);		// ƥ�������ַ���str



	//regex e("^abc.");						// ^	begin of the string		������abc��ͷ�����ַ���
	//regex e("abc.$");						// $	end of the string		������abc��β�����ַ���
	regex e("^abc.+");

	bool match = regex_search(str, e);		// �����ַ���str��ƥ��e��������ַ���

											// ���Ҫ�滻���������������ʽ������regex�ĵڶ�������ָ��
											//regex egrep("^abc.+", regex_constants::grep);

	cout << (match ? "Matched" : "Not matched") << endl << endl;
}

void test_submatch(const string& str)
{
	smatch m;

	// �����Ҫ��֧ƥ�䣬��Ҫ��������()������
	// ԭ��䣺regex e("[[:w:]]+@[[:w:]]+\.com");
	// ��@ǰ����������������ʾҪ��ȡ�û���������
	regex e("([[:w:]]+)@([[:w:]]+)\.com");

	bool found = regex_search(str, m, e);
	for (int n = 0; n < m.size(); ++n)
	{
		cout << "m[" << n << "].str()=" << m[n].str() << endl;
		//cout << "m[" << n << "].str()=" << m.str(n) << endl;
		//cout << "m[" << n << "].str()=" << *(m.begin() + n) << endl;
	}

	// prefix��suffixָ������ƥ����ַ���ǰ����ַ���
	// ���磺<email>ping@163.com<end>
	// ��ʱ��<email>����prefix��<end>����suffix
	cout << "m.prefix.str()=" << m.prefix().str() << endl;
	cout << "m.suffix.str()=" << m.suffix().str() << endl;
}

void test_iterator()
{
	string str = "ping@163.com;			ping123@gmail.com;;;	ping456@qq.com";

	regex e("([[:w:]]+)@([[:w:]]+)\.com");

	smatch m;

	// �����������Ѿ���������Ҫ��
	//bool found = regex_search(str, m, e);
	//for (int n = 0; n < m.size(); ++n)
	//{
	//	cout << "m[" << n << "].str()=" << m[n].str() << endl;
	//}


	// ����1
	//sregex_iterator pos(str.cbegin(), str.end(), e);
	//sregex_iterator end;		// Default constructor defines past-the-end iterator
	//for (; pos != end; ++pos)
	//{
	//	cout << "matched: " << pos->str(0) << endl;
	//	cout << "user name: " << pos->str(1) << endl;
	//	cout << "domain: " << pos->str(2) << endl;
	//	cout << endl;
	//}


	// ����2
	// ���һ������ָ��token�����ݡ������0����ƥ��ȫ��
	// 1�Ļ����ǵ�һ����������Ҳ���ǣ��������Դ�����
	//sregex_token_iterator pos(str.cbegin(), str.end(), e, 3);
	//sregex_token_iterator end;		// Default constructor defines past-the-end iterator
	//for (; pos != end; ++pos)
	//{
	//	cout << "matched: " << pos->str() << endl;
	//	cout << endl;
	//}


	// �滻
	cout << regex_replace(str, e, "$1 is on $2");

	// �����ƥ����ַ�����Ҫ�����Ļ���ָ������������Ϊregex_constants::format_no_copy
	//cout << regex_replace(str, e, "$1 is on $2", regex_constants::format_no_copy);

	cout << "=====================================\n\n";
}

void test()
{
	string str = "xxxxx(\"asdaSD\")";
	//string str = "aaaa(\"";

	//regex e("(\\w+)\\s{1}\\(\"(\\w+)\"\\)");
	regex e("([a-zA-Z]+)\\s?\\(\"([a-zA-Z]+)\"\\)");

	if (regex_match(str, e))
	{
		smatch m;
		auto found = regex_search(str, m, e);
		for (int n = 0; n < m.size(); ++n)
		{
			cout << "m[" << n << "].str()=" << m[n].str() << endl;
		}
	}
	else
	{
		cout << "Not matched" << endl;
	}
}

int main()
{
	// default: ECMAScript

	string str;
	while (true)
	{
		cin >> str;
		
		//test_basic(str);

		//test_submatch(str);

		//test_iterator();

		test();
	}
}