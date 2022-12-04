#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <list>

using NodeRef = std::shared_ptr<struct Node>;

struct Node
{
public:
	Node() {}
	Node(const std::string& data) : data(data) {}

public:
	std::string				data;
	std::vector<NodeRef>	_children;
};

NodeRef CreateTree()
{
	NodeRef root = std::make_shared<Node>("R1 ���߽�");
	{
		NodeRef node = std::make_shared<Node>("��������");
		root->_children.emplace_back(node);

		{
			NodeRef leaf = std::make_shared<Node>("����");
			node->_children.emplace_back(leaf);
		}

		{
			NodeRef leaf = std::make_shared<Node>("����");
			node->_children.emplace_back(leaf);
		}

		{
			NodeRef leaf = std::make_shared<Node>("���丮");
			node->_children.emplace_back(leaf);
		}
	}

	{
		NodeRef node = std::make_shared<Node>("���α׷�����");
		root->_children.emplace_back(node);

		{
			NodeRef leaf = std::make_shared<Node>("����");
			node->_children.emplace_back(leaf);
		}

		{
			NodeRef leaf = std::make_shared<Node>("Ŭ��");
			node->_children.emplace_back(leaf);
		}

		{
			NodeRef leaf = std::make_shared<Node>("����");
			node->_children.emplace_back(leaf);
		}
	}

	{
		NodeRef node = std::make_shared<Node>("��Ʈ��");
		root->_children.emplace_back(node);

		{
			NodeRef leaf = std::make_shared<Node>("���");
			node->_children.emplace_back(leaf);

			{
				NodeRef leaf2 = std::make_shared<Node>("sex");
				leaf->_children.emplace_back(leaf2);
			}
		}

		{
			NodeRef leaf = std::make_shared<Node>("ĳ����");
			node->_children.emplace_back(leaf);
		}
	}

	return root;
}

void PrintTree(NodeRef root, int depth)
{
	for (int i = 0; i < depth; ++i)
		std::cout << "-";
	std::cout << root->data << std::endl;

	for (NodeRef& child : root->_children)
		PrintTree(child, depth + 1);
}


int GetTreeHeight(const NodeRef& root, int height = 1)
{
	if (root->_children.empty())
		return height;

	for (NodeRef& c : root->_children)
	{
		return GetTreeHeight(c, height + 1);
	}
}

int GetHeight(NodeRef root)
{
	int height = 1;
	
	for (NodeRef& child : root->_children)
	{
		height = std::max(height, GetHeight(child) + 1);
	}

	return height;
}

int GetTreeDepth(const NodeRef& root, int depth = 0)
{
	if (root->_children.empty())
		return depth;

	for (NodeRef& c : root->_children)
	{
		return GetTreeDepth(c, depth + 1);
	}
}

int GetDepth(const NodeRef& root)
{
	int depth = 0;

	for (NodeRef& child : root->_children)
	{
		depth = std::max(depth, GetDepth(child) + 1);
	}

	return depth;
}

void main()
{
	NodeRef root = CreateTree();

	PrintTree(root, 0);

	// int depth = GetTreeHeight(root);
	int height = GetHeight(root);

	std::cout << "Tree ���� : "  << height << std::endl;

	int depth = GetDepth(root);

	std::cout << "Tree ���� : " << depth << std::endl;

	return;
}