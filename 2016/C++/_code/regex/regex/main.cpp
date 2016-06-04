// youtube  -> Bo Qian

#include <regex>
#include <iostream>
#include <string>

using namespace std;

void test_basic(const string& str)
{
	//regex e("abc.", regex_constants::icase);		// .  Any character except newline. 1个字符
	//regex e("abc?");						// ?		Zero or 1 preceding character. 匹配?前一个字符
	//regex e("abc*");						// *		Zero or more preceding character. 匹配*前一个字符
	//regex e("abc+");						// +		One or more preceding character. 匹配+前一个字符
	//regex e("ab[cd]*");					// [...]	Any character inside square brackets. 匹配[]内的任意字符
	//regex e("ab[^cd]*");					// [...]	Any character not inside square brackets. 匹配非[]内的任意字符
	//regex e("ab[cd]{3}");					// {n}		匹配{}之前任意字符，且字符个数为3个
	//regex e("ab[cd]{3,}");				// {n}		匹配{}之前任意字符，且字符个数为3个或3个以上
	//regex e("ab[cd]{3,5}");				// {n}		匹配{}之前任意字符，且字符个数为3个以上，5个以下闭区间
	//regex e("abc|de[fg]");				// |		匹配|两边的任意一个规则
	//regex e("(abc)de+\\1");				// ()       ()表示一个子分组，而\1表示在此位置匹配第一个分组的内容
	//regex e("(abc)c(de+)\\2\\1");			// \2		表示的是在此匹配第二个分组的内容
	//regex e("[[:w:]]+@[[:w:]]+\.com");	//	[[:w:]]	word character: digit, number, underscore

	//bool match = regex_match(str, e);		// 匹配整个字符串str



	//regex e("^abc.");						// ^	begin of the string		查找以abc开头的子字符串
	//regex e("abc.$");						// $	end of the string		查找以abc结尾的子字符串
	regex e("^abc.+");

	bool match = regex_search(str, e);		// 查找字符串str中匹配e规则的子字符串

											// 如果要替换其他规则的正则表达式，可在regex的第二个参数指定
											//regex egrep("^abc.+", regex_constants::grep);

	cout << (match ? "Matched" : "Not matched") << endl << endl;
}

void test_submatch(const string& str)
{
	smatch m;

	// 如果需要分支匹配，需要将分组以()括起来
	// 原语句：regex e("[[:w:]]+@[[:w:]]+\.com");
	// 将@前后的语句括起来，表示要获取用户名和域名
	regex e("([[:w:]]+)@([[:w:]]+)\.com");

	bool found = regex_search(str, m, e);
	for (int n = 0; n < m.size(); ++n)
	{
		cout << "m[" << n << "].str()=" << m[n].str() << endl;
		//cout << "m[" << n << "].str()=" << m.str(n) << endl;
		//cout << "m[" << n << "].str()=" << *(m.begin() + n) << endl;
	}

	// prefix和suffix指的是在匹配的字符串前后的字符串
	// 比如：<email>ping@163.com<end>
	// 此时，<email>就是prefix，<end>就是suffix
	cout << "m.prefix.str()=" << m.prefix().str() << endl;
	cout << "m.suffix.str()=" << m.suffix().str() << endl;
}

void test_iterator()
{
	string str = "ping@163.com;			ping123@gmail.com;;;	ping456@qq.com";

	regex e("([[:w:]]+)@([[:w:]]+)\.com");

	smatch m;

	// 上述的例子已经不能满足要求
	//bool found = regex_search(str, m, e);
	//for (int n = 0; n < m.size(); ++n)
	//{
	//	cout << "m[" << n << "].str()=" << m[n].str() << endl;
	//}


	// 方案1
	//sregex_iterator pos(str.cbegin(), str.end(), e);
	//sregex_iterator end;		// Default constructor defines past-the-end iterator
	//for (; pos != end; ++pos)
	//{
	//	cout << "matched: " << pos->str(0) << endl;
	//	cout << "user name: " << pos->str(1) << endl;
	//	cout << "domain: " << pos->str(2) << endl;
	//	cout << endl;
	//}


	// 方案2
	// 最后一个参数指定token的内容。如果是0，则匹配全部
	// 1的话就是第一个子条件（也就是（）），以此类推
	//sregex_token_iterator pos(str.cbegin(), str.end(), e, 3);
	//sregex_token_iterator end;		// Default constructor defines past-the-end iterator
	//for (; pos != end; ++pos)
	//{
	//	cout << "matched: " << pos->str() << endl;
	//	cout << endl;
	//}


	// 替换
	cout << regex_replace(str, e, "$1 is on $2");

	// 如果不匹配的字符不需要拷贝的话，指定第三个参数为regex_constants::format_no_copy
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