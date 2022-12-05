#pragma once

struct NodeB
{
	NodeB*	parent = nullptr;
	NodeB*	left = nullptr;
	NodeB*	right = nullptr;
	int		data = {};
};

class Bst
{
public:
	void	Print() { Print(_root, 10, 0); }
	void	Print(NodeB* node, int x, int y);
	void	Print_Inorder() { Print_Inorder(_root); }
	void	Print_Inorder(NodeB* node);

	NodeB*	Search(NodeB* node, int key);
	NodeB*	Search2(NodeB* node, int key);

	NodeB*	Min(NodeB* node);
	NodeB*	Max(NodeB* node);
	NodeB*	Next(NodeB* node);

	void	Insert(int key);
	void	Delete(int key);
	void	Delete(NodeB* node);
	void	Replace(NodeB* u, NodeB* v);

private:
	NodeB* _root = nullptr;
};

