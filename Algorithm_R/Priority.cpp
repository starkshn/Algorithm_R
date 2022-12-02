#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <list>
using namespace std;

template <typename T>
struct Node
{
public:
	T		 _data = -1;
	Node<T>* _parent = nullptr;
	Node<T>* _rightChild = nullptr;
	Node<T>* _leftChild = nullptr;

public:
	Node() { cout << "Node 기본 생성자 호출 " << endl; }

public:
	Node<T>& operator = (const Node<T>& other)
	{
		this->_data = other._data;
		this->_parent = other._parent;
		this->_leftChild = other._leftChild;
		this->_rightChild = other._rightChild;

		return *this;
	}

	//bool operator == (const Node<T>& other)
	//{

	//}

	//bool operator != (const Node<T>& other)
	//{

	//}

	//bool operator < (const Node<T>& other)
	//{

	//}

	//bool operator > (const Node<T>& other)
	//{

	//}



};


template <typename T>
class PQ
{
private:
	std::vector<Node<T>*>	_pq;
	Node<T>*				_rootNode = nullptr;

public:
	PQ() { _pq.reserve(20);  cout << "PQ 기본 생성자 호출 " << endl; }
	~PQ() 
	{
		for (size_t i = 0; i < _pq.size(); ++i)
			delete _pq[i];
	}

public:
	void push(const T& data)
	{
		if (_rootNode == nullptr)
		{
			_rootNode = new Node<T>;
			_rootNode->_data = data;
			_pq.push_back(_rootNode);
		}
		else
		{
			// 데이터 넣기
			for (size_t i = 0; i < _pq.size(); ++i)
			{
				// left 있을 경우
				if (_pq[i]->_leftChild != nullptr)
				{
					if (_pq[i]->_rightChild != nullptr)
						continue;
					else
					{
						Node<T>* node =  new Node<T>;
						node->_data = data;
						node->_parent = _pq[i];
						_pq.push_back(node);
						break;
					}
				}
				// left없을경우
				else
				{
					Node<T>* node = new Node<T>;
					node->_data = data;
					node->_parent = _pq[i];
					_pq[i]->_leftChild = node;
					_pq.push_back(node);
					break;
				}
			}

			// 도장깨기
			while (true)
			{
				
			}
		}
	}

	void	pop()	{ _pq.erase(_pq.front()); }
	T		top()	{ return _pq.front(); }
	T		empty() { return _pq.empty(); }
	int		size()	{ return _pq.size(); }
	
public:
	T GetLeftChild(int idx)
	{
		return _pq[(2 * idx) + 1]->_data;
	}

	T GetRightChild(int idx)
	{

		return _pq[2 * idx + 2]->_data;
	}

	T GetParent(int idx)
	{

		return _pq[(idx - 1) / 2]->_data;
	}

public:
	

};


int main()
{
	PQ<int> pq;
	pq.push(200);
	pq.push(300);
	pq.push(500);
	pq.push(400);
	pq.push(700);
	pq.push(800);
	

	cout << pq.GetLeftChild(0) << endl;
	cout << pq.GetRightChild(0) << endl;
	cout << pq.GetParent(4) << endl;


	int a = 10;
	

	return 0;
}