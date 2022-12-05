#include "RedBlackTree.h"
#include <iostream>
#include <windows.h>
using namespace std;

enum class ConsoleColor
{
	BLACK = 0,
	RED = FOREGROUND_RED,
	GREEN = FOREGROUND_GREEN,
	BLUE = FOREGROUND_BLUE,
	YELLOW = RED | GREEN,
	WHITE = RED | GREEN | BLUE,
};

void SetCursorColor(ConsoleColor color)
{
	HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
	::SetConsoleTextAttribute(output, static_cast<SHORT>(color));
}

void SetCursorPosition(int x, int y)
{
	HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
	::SetConsoleCursorPosition(output, pos);
}

RedBlackTree::RedBlackTree()
{
	_nil = new Node();
	_root = _nil;
}

RedBlackTree::~RedBlackTree()
{
	delete _nil;
}

void RedBlackTree::Print(Node* node, int x, int y)
{
	if (node == _nil)
		return;

	SetCursorPosition(x, y);

	if (node->color == Color::Black)
		SetCursorColor(ConsoleColor::BLUE);
	else
		SetCursorColor(ConsoleColor::RED);

	cout << node->data;
	Print(node->left, x - (8 / (y + 1)), y + 1);
	Print(node->right, x + (8 / (y + 1)), y + 1);

	// �ǹ� ���� �κ�
	SetCursorColor(ConsoleColor::WHITE);
}

// ��ͷ� ã�� �Լ�
Node* RedBlackTree::Search(Node* node, int data)
{
	if (node == _nil || data == node->data)
		return node;

	if (data < node->data)
		return Search(node->left, data);
	else
		return Search(node->right, data);
}

Node* RedBlackTree::Min(Node* node)
{
	while (node->left)
		node = node->left;

	return node;
}

Node* RedBlackTree::Max(Node* node)
{
	while (node->right)
		node = node->right;

	return node;
}

Node* RedBlackTree::Next(Node* node)
{
	if (node->right != _nil)
		return Min(node->right);

	Node* parent = node->parent;

	while (parent != _nil && node == parent->right)
	{
		node = parent;
		parent = parent->parent;
	}

	return parent;
}

void RedBlackTree::Insert(int data)
{
	Node* newNode = new Node();
	newNode->data = data;

	Node* node = _root;
	Node* parent = _nil;

	while (node != _nil)
	{
		parent = node;
		if (data < node->data)
			node = node->left;
		else
			node = node->right;
	}

	newNode->parent = parent;
	
	if (parent == _nil)
		_root = newNode;
	else if (data < parent->data)
		parent->left = newNode;
	else
		parent->right = newNode;

	// �˻�
	newNode->left = _nil;
	newNode->right = _nil;
	newNode->color = Color::Red; // ���� �߰��� Node�� �׻� RED
	
	InsertFixup(newNode);
}

void RedBlackTree::InsertFixup(Node* node)
{
	// 1) p = red, uncle = red
	// -> p = black, uncle = black, pp = red�� �ٲ�
	// 2) p = red, uncle = black (triangle)
	// -> ȸ���� ���� case 3���� �ٲ�
	// 3) p = red, uncle = black (list)	
	// -> ���� ���� + ȸ��

	while (node->parent->color == Color::Red)
	{
		// ���� n�� p�� pp�� ���� �ڽ��� ���
		if (node->parent == node->parent->parent->left)
		{
			Node* uncle = node->parent->parent->right; // uncle���ٸ� _nil
			if (uncle->color == Color::Red)
			{
				node->parent->color = Color::Black; // p
				uncle->color = Color::Black; // u
				node->parent->parent->color = Color::Red; // pp
				node = node->parent->parent;
			}
			else
			{
				if (node == node->parent->right) // Triangle Ÿ��
				{
					node = node->parent;
					LeftRotate(node);
				}

				node->parent->color = Color::Black;
				node->parent->parent->color = Color::Red;


				RightRotate(node->parent->parent);
			}
		}
		else
		{
			Node* uncle = node->parent->parent->left;
			if (uncle->color == Color::Red)
			{
				node->parent->color = Color::Black; // p
				uncle->color = Color::Black; // u
				node->parent->parent->color = Color::Red; // pp
				node = node->parent->parent;
			}
			else
			{
				if (node == node->parent->left) // Triangle Ÿ��
				{
					node = node->parent;
					RightRotate(node);
				}
				node->parent->color = Color::Black;
				node->parent->parent->color = Color::Red;
				LeftRotate(node->parent->parent);
			}
		}
	}

	_root->color = Color::Black;
}

void RedBlackTree::Delete(int data)
{
	Node* deleteNode = Search(_root, data);
	Delete(deleteNode);
}

void RedBlackTree::Delete(Node* node)
{
	if (node == nullptr)
		return;
	if (node->left == nullptr)
		Replace(node, node->right);
	else if (node->right == nullptr)
		Replace(node, node->left);
	else
	{
		Node* next = Next(node);
		node->data = next->data;
		Delete(next);
	}
}


// u ����Ʈ���� v ����Ʈ���� ��ü
// �׸��� delete u
void RedBlackTree::Replace(Node* u, Node* v)
{
	if (!u->parent)
		_root = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else
		u->parent->right = v;

	if (v)
		v->parent = u->parent;

	delete u;
}

//     [y]
//  [x]   [3]
// [1][2]

//    [x]  
// [1]   [y]
//      [2][3]



//		[y : 20] 
//	[x : 10]


//		[x : 10]
//			[y : 20]

void RedBlackTree::LeftRotate(Node* x)
{
	Node* y = x->right; // 

	x->right = y->left; // [2]

	// ���� �ڽ��� �θ�
	if (y->left != _nil)
		y->left->parent = x;

	y->parent = x->parent;

	if (x->parent == _nil)
		_root = y;
	else if (x == x->parent->left)	// x�� �θ��� ���� �ڽ��� ���
		x->parent->left = y;
	else
		x->parent->right = y;		// x�� �θ��� ������ �ڽ��� ��� 

	y->left = x;
	x->parent = y;
}

//     [y]
//  [x]   [3]
// [1][2]

//    [x]  
// [1]   [y]
//      [2][3]
void RedBlackTree::RightRotate(Node* y)
{
	Node* x = y->left;

	y->left = x->right;

	if (x->right != _nil)
		x->right->parent = y;

	x->parent = y->parent;

	if (y->parent == _nil)
		_root = x;
	else if (y == y->parent->left)
		y->parent->left = x;
	else
		y->parent->right = x;

	x->right = y;
	y->parent = x;
}




