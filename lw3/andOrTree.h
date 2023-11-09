#ifndef _ANDORTREE_H_
#define _ANDORTREE_H_

#include <string>
#include <fstream>
#include <vector>
#include <stack>

class AndOrTreeNode
{
 public:
	AndOrTreeNode(int level, std::string key, char type)
		: key(key), type(type), level(level)
	{
	}

	void outputNodeToStream(std::ofstream& ofs);

	std::string key;
	char type;
	AndOrTreeNode* fath = nullptr;
	int level;
	bool isOutput = false;
	std::vector<AndOrTreeNode*> sons {};
};

class AndOrTree
{
 public:
	AndOrTree()
		: Root()
	{
	}

	void ReadTreeFromStream(std::ifstream& ifs);

	void OutputTreeToStream(std::ofstream& ofs, AndOrTreeNode* ptr);

	void OutputTreeToStream(std::ofstream& ofs);

	bool OutputTreesToStream(std::ofstream& ofs, std::stack<AndOrTreeNode*>& orNodesOutputLeaves);

	bool isAllTreesOutput();

	void resetNodesToRightOfPtr(AndOrTreeNode* ptr);

	~AndOrTree();

 private:
	AndOrTreeNode* Root = nullptr;

	AndOrTreeNode* Insert(int level, int lastLevel, AndOrTreeNode* lastNode, std::string str, char type);

	void OutputTreesToStream(std::ofstream& ofs, AndOrTreeNode* ptr, std::stack<AndOrTreeNode*>& orNodesOutputLeaves);

	bool isAllTreesOutput(AndOrTreeNode* ptr);

	static bool checkNodeForOutput(AndOrTreeNode* ptr);

	bool resetNodesToRightOfPtr(AndOrTreeNode* node, AndOrTreeNode* ptr);
};

#endif //_ANDORTREE_H_
