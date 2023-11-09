#include "andOrTree.h"
#include <algorithm>

using namespace std;

static void DestroyNode(AndOrTreeNode* node)
{
	if (node)
	{
		for (AndOrTreeNode* ptr : node->sons)
		{
			DestroyNode(ptr);
		}
		delete node;
	}
}

AndOrTree::~AndOrTree()
{
	DestroyNode(Root);
}

void AndOrTreeNode::outputNodeToStream(std::ofstream& ofs)
{
	string str = this->key;
	AndOrTreeNode* tmpPtr = this;
	for (int i = 0; i < this->level; i++)
	{
		tmpPtr = tmpPtr->fath;
		str = "." + str;
	}
	if (this->level > 0)
	{
		ofs << endl;
	}
	ofs << str << " " << this->type;
}

AndOrTreeNode* AndOrTree::Insert(int level, int lastLevel, AndOrTreeNode* lastNode, string key, char type)
{
	auto newNode = new AndOrTreeNode(level, key, type);
	if (level == 0)
	{
		this->Root = newNode;
		return newNode;
	}
	if (level > lastLevel)
	{
		newNode->fath = lastNode;
		lastNode->sons.push_back(newNode);
		return newNode;
	}
	AndOrTreeNode* tmpPtr = lastNode;
	for (int i = 0; i < lastLevel - level; i++)
	{
		tmpPtr = tmpPtr->fath;
	}
	newNode->fath = tmpPtr->fath;
	newNode->fath->sons.push_back(newNode);
	return newNode;
}

void AndOrTree::ReadTreeFromStream(ifstream& ifs)
{
	string str;
	int lastLevel = 0;
	AndOrTreeNode* lastNode = nullptr;
	while (getline(ifs, str))
	{
		int level = 0;
		string key{};
		char type;
		while (str[0] == '.')
		{
			level++;
			str = str.substr(1);
		}
		while (str[0] != ' ')
		{
			key += str[0];
			str = str.substr(1);
		}
		while (str[0] == ' ')
		{
			str = str.substr(1);
		}
		type = str[0];
		lastNode = this->Insert(level, lastLevel, lastNode, key, type);
		lastLevel = level;
	}
}

void AndOrTree::OutputTreeToStream(std::ofstream& ofs)
{
	if (AndOrTree::Root == nullptr)
	{
		return;
	}
	AndOrTree::Root->outputNodeToStream(ofs);
	for (AndOrTreeNode* son : AndOrTree::Root->sons)
	{
		this->OutputTreeToStream(ofs, son);
	}
}

void AndOrTree::OutputTreeToStream(std::ofstream& ofs, AndOrTreeNode* ptr)
{
	if (ptr == nullptr)
	{
		return;
	}
	string str = ptr->key;
	ptr->outputNodeToStream(ofs);
	for (AndOrTreeNode* son : ptr->sons)
	{
		this->OutputTreeToStream(ofs, son);
	}
}

bool AndOrTree::isAllTreesOutput()
{
	if (AndOrTree::Root == nullptr)
	{
		return true;
	}
	if (AndOrTree::Root->type == 'a')
	{
		return this->checkNodeForOutput(this->Root);
	}
	else if (AndOrTree::Root->type == 'o' && !AndOrTree::Root->isOutput)
	{
		return AndOrTree::Root->isOutput;
	}
	return true;
}

bool AndOrTree::isAllTreesOutput(AndOrTreeNode* ptr)
{
	if (ptr == nullptr)
	{
		return true;
	}
	if (ptr->type == 'a')
	{
		for (AndOrTreeNode* son : ptr->sons)
		{
			if (!AndOrTree::isAllTreesOutput(son))
			{
				return false;
			}
		}
	}
	else if (ptr->type == 'o' && !ptr->isOutput)
	{
		return this->checkNodeForOutput(ptr);
	}
	return true;
}

bool AndOrTree::checkNodeForOutput(AndOrTreeNode* ptr)
{
	if (ptr->type == 'l')
	{
		return ptr->isOutput;
	}
	bool isAllSonsOutput = true;
	for (AndOrTreeNode* son : ptr->sons)
	{
		if (!son->isOutput)
		{
			isAllSonsOutput = AndOrTree::checkNodeForOutput(son);
			if (!isAllSonsOutput)
			{
				break;
			}
		}
	}
	ptr->isOutput = isAllSonsOutput;
	return isAllSonsOutput;
}

void AndOrTree::resetNodesToRightOfPtr(AndOrTreeNode* ptr)
{
	if (this->Root == nullptr || this->Root == ptr)
	{
		return;
	}
	this->Root->isOutput = false;
	vector<AndOrTreeNode*> tmpV = this->Root->sons;
	reverse(tmpV.begin(), tmpV.end());
	for (AndOrTreeNode* son : tmpV)
	{
		if (this->resetNodesToRightOfPtr(son, ptr))
		{
			return;
		}
	}
}

bool AndOrTree::resetNodesToRightOfPtr(AndOrTreeNode* node, AndOrTreeNode* ptr)
{
	if (node == nullptr || node == ptr)
	{
		return true;
	}

	node->isOutput = false;
	vector<AndOrTreeNode*> tmpV = node->sons;
	reverse(tmpV.begin(), tmpV.end());
	for (AndOrTreeNode* son : tmpV)
	{
		if (this->resetNodesToRightOfPtr(son, ptr))
		{
			return true;
		}
	}
	return false;
}

bool AndOrTree::OutputTreesToStream(std::ofstream& ofs, stack<AndOrTreeNode*>& orNodesOutputLeaves)
{
	if (this->Root == nullptr)
	{
		return true;
	}
	this->Root->outputNodeToStream(ofs);
	if (this->Root->type == 'a')
	{
		for (AndOrTreeNode* son : this->Root->sons)
		{
			AndOrTree::OutputTreesToStream(ofs, son, orNodesOutputLeaves);
		}
	}
	else if (AndOrTree::Root->type == 'o' && !AndOrTree::Root->isOutput)
	{
		for (AndOrTreeNode* son : AndOrTree::Root->sons)
		{
			if (!son->isOutput)
			{
				AndOrTree::OutputTreesToStream(ofs, son, orNodesOutputLeaves);
				break;
			}
		}
	}
	if (!orNodesOutputLeaves.empty())
	{
		auto tmpNode = orNodesOutputLeaves.top();
		AndOrTreeNode* prevNode = nullptr;
		orNodesOutputLeaves.pop();
		tmpNode->isOutput = true;
		while ((this->checkNodeForOutput(tmpNode->fath) || tmpNode->fath->type == 'a') && !orNodesOutputLeaves.empty())
		{
			tmpNode = orNodesOutputLeaves.top();
//			if (prevNode && prevNode->fath != tmpNode->fath)
//			{
//				if ((prevNode && prevNode->fath && prevNode->fath != tmpNode && !prevNode->fath->isOutput)
//					|| (prevNode && !prevNode->fath && !prevNode->isOutput))
//				{
//					break;
//				}
//			}
			;
			tmpNode->isOutput = true;
			prevNode = tmpNode;
			orNodesOutputLeaves.pop();
		}
		if (this->isAllTreesOutput())
		{
			return true;
		}
		this->resetNodesToRightOfPtr(tmpNode->fath);
		while (!orNodesOutputLeaves.empty())
		{
			orNodesOutputLeaves.pop();
		}
	}
	return false;
}

void AndOrTree::OutputTreesToStream(std::ofstream& ofs, AndOrTreeNode* ptr, stack<AndOrTreeNode*>& orNodesOutputLeaves)
{
	if (ptr == nullptr)
	{
		return;
	}
	ptr->outputNodeToStream(ofs);
	if (ptr->type == 'a')
	{
		orNodesOutputLeaves.push(ptr);
		for (AndOrTreeNode* son : ptr->sons)
		{
			AndOrTree::OutputTreesToStream(ofs, son, orNodesOutputLeaves);
		}
	}
	else if (ptr->type == 'o' && !ptr->isOutput)
	{
		orNodesOutputLeaves.push(ptr);
		//TODO Exception if no sons
		for (AndOrTreeNode* son : ptr->sons)
		{
			if (!son->isOutput)
			{
				AndOrTree::OutputTreesToStream(ofs, son, orNodesOutputLeaves);
				break;
			}
		}
		this->checkNodeForOutput(ptr);
	}
	if (ptr->type == 'l')
	{
//		if (ptr->fath->type == 'a')
//		{
//			if (ptr->fath->sons[0] == ptr)
//			{
//				orNodesOutputLeaves.push(ptr->fath);
//			}
//		}
		orNodesOutputLeaves.push(ptr);
	}
}