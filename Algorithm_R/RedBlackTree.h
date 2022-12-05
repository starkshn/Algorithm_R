#pragma once
// �⺻������ BST���� ������ ���ߴ� �˰����� �� BST�̴�.

enum class Color
{
	Red = 0,
	Black = 1,
};

struct Node
{
	Node*	parent = nullptr;
	Node*	left = nullptr;
	Node*	right = nullptr;
	int		data = {};
	Color	color = Color::Black;
};

class RedBlackTree
{
public:
	RedBlackTree();
	~RedBlackTree();

public:
	void	Print() { Print(_root, 20, 0); }
	void	Print(Node* node, int x, int y);

	Node*	Search(Node* node, int key);

	Node*	Min(Node* node);
	Node*	Max(Node* node);
	Node*	Next(Node* node);

	void	Insert(int key);
	void	InsertFixup(Node* node);

	void	Delete(int key);
	void	Delete(Node* node);
	void	Replace(Node* u, Node* v);

	// Red-Black Tree
	void	LeftRotate(Node* x);
	void	RightRotate(Node* y);

private:
	Node* _root = nullptr;
	Node* _nil = nullptr;
};

