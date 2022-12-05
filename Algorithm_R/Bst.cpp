#include "Bst.h"
#include <iostream>
#include <windows.h>
using namespace std;

void SetCursorPosition(int x, int y)
{
	HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
	::SetConsoleCursorPosition(output, pos);
}

void Bst::Print(NodeB* node, int x, int y)
{
	if (node == nullptr)
		return;

	SetCursorPosition(x, y);

	cout << node->data;
	Print(node->left, x - (5 / (y + 1)), y + 1);
	Print(node->right, x + (5 / (y + 1)), y + 1);
}

void Bst::Print_Inorder(NodeB* node)
{
	// 전위 순회 (preorder traverse)
	// 중위 순회 (inorder)
	// 후위 순회 (postorder)
	if (node == nullptr)
		return;

	// 전위 : [중]이 앞에 온다
	// 중위 : [중]이 중간에 온다
	// 후위 : [중]이 마지막에 온다	
	cout << node->data << endl;
	Print_Inorder(node->left);
	Print_Inorder(node->right);
}

// 재귀로 찾는 함수
NodeB* Bst::Search(NodeB* node, int data)
{
	if (node == nullptr || data == node->data)
		return node;

	if (data < node->data)
		return Search(node->left, data);
	else
		return Search(node->right, data);
}

// 그냥? 찾는 함수
// 재귀보다 성능면에서 좋다.
NodeB* Bst::Search2(NodeB* node, int data)
{
	while (node && data != node->data)
	{
		if (data < node->data)
			node = node->left;
		else
			node = node->right;
	}

	return node;
}

NodeB* Bst::Min(NodeB* node)
{
	while (node->left)
		node = node->left;

	return node;
}

NodeB* Bst::Max(NodeB* node)
{
	while (node->right)
		node = node->right;

	return node;
}

NodeB* Bst::Next(NodeB* node)
{
	if (node->right)
		return Min(node->right);

	NodeB* parent = node->parent;

	while (parent && node == parent->right)
	{
		node = parent;
		parent = parent->parent;
	}
	
	return parent;
}

void Bst::Insert(int data)
{
	NodeB* newNode = new NodeB();
	newNode->data = data;
	
	if (!_root)
	{
		_root = newNode;
		return;
	}

	NodeB* node = _root;
	NodeB* parent = nullptr;

	while (node)
	{
		parent = node;
		if (data < node->data)
			node = node->left;
		else
			node = node->right;
	}

	newNode->parent = parent;

	if (data < parent->data)
		parent->left = newNode;
	else
		parent->right = newNode;
}

void Bst::Delete(int data)
{
	NodeB* deleteNode = Search(_root, data);
	Delete(deleteNode);
}

void Bst::Delete(NodeB* node)
{
	if (node == nullptr)
		return;
	if (node->left == nullptr)
		Replace(node, node->right);
	else if (node->right == nullptr)
		Replace(node, node->left);
	else
	{
		NodeB* next = Next(node);
		node->data = next->data;
		Delete(next);
	}
}


// u 서브트리를 v 서브트리로 교체
// 그리고 delete u
void Bst::Replace(NodeB* u, NodeB* v)
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
