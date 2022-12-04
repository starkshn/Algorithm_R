#include <iostream>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

std::vector<int> vec;

void B(int v)
{
	int left = 0;
	int right = vec.size() - 1;

	int count = 0;

	while (left <= right)
	{
		int mid = (left + right) / 2;
		
		cout << "���� Ƚ�� : " << count << endl;
		cout << "���� : " << left << "~" << right << endl;
		
		if (v < vec[mid])
		{
			right = mid - 1;
		}
		else if (v > vec[mid])
		{
			left = mid + 1;
		}
		else
		{
			cout << "find : " << v << endl;
			break;
		}
		
		++count;
	}
}


int main()
{
	vec = { 10, 20, 30, 40, 100, 500, 60, 50, 70, 99, 13, 14, 15 , 16 , 18};
	B(10);

	return 0;
}