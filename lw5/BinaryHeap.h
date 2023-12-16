#ifndef ALGOS_BINARYHEAP_H
#define ALGOS_BINARYHEAP_H

#include <vector>

class BinaryHeap
{
public:
	bool empty();
	std::pair<int, int> top();
	void push(std::pair<int, int> node);
	void pop();

private:
	std::vector<std::pair<int, int>> value{};

	void siftUp(int index);
	void heapify(int index);
	int parent(int index);
};


#endif //ALGOS_BINARYHEAP_H
