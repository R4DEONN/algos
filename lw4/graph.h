//
// Created by admin on 16.11.2023.
//

#ifndef ALGOS_GRAPH_H
#define ALGOS_GRAPH_H

#include <string>

class GraphNode
{
public:
	GraphNode(int nameId, int inputId, int outputId)
	: nameId(nameId), inputId(inputId), outputId(outputId)
	{};

private:
	int nameId;
	int inputId;
	int outputId;
};


#endif //ALGOS_GRAPH_H
