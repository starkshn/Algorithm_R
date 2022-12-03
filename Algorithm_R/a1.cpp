#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>
using namespace std;

// 22/12/02

struct IsOdd
{
	bool operator () (int n)
	{
		return n % 2 == 1;
	}
};

struct IsEven
{
	bool operator () (int n)
	{
		return n % 2 == 0;
	}
};

template <typename TYPE = int>
class Test
{
public:
	Test() {}
	Test(initializer_list<TYPE> li) 
	{	
		vector<TYPE> v2{li};
		cout << v2[0] << endl;
		std::for_each(v2.begin(), v2.end(), [](int n) { std::cout << n << std::endl; });
	}

private:
	vector<TYPE> v;
	
	
};

class Test3 {};

class Test2
{
public:
	int a = 100;

public:
	Test3* t;
};


int main()
{
	Test3* t3t = new Test3();

	Test2 t2t;
	Test2 t2t2;

	t2t.t = t3t;

	int a = 10;

	t2t2 = std::move(t2t);


	int b = 10;

	return 0;
}