#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <list>

using NodeRef = std::shared_ptr<struct Node>;

struct Node
{
	Node() {}
	Node(const std::string& data) : _data(data) {}
	
	std::string _data;
	std::vector<NodeRef> _children;
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

int GetHeight(const NodeRef& root)
{
	int height = 1;
	
	for (NodeRef& childe : root->_children)
		height = std::max(height, GetHeight(childe) + 1);

	return height;
}

void PrintTree(NodeRef root, int height)
{
	for (int i = 0; i < height; ++i)
		std::cout << "-";
	
	std::cout << root->_data << std::endl;

	for (NodeRef& child : root->_children)
		PrintTree(child, height + 1);
}

void main()
{
	NodeRef root = CreateTree();

	PrintTree(root, 0);

	int height = GetHeight(root);

	std::cout << "Tree ���� : " << height << std::endl;

	return;
}