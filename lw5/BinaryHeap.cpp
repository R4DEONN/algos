#include "BinaryHeap.h"

std::pair<int, int> BinaryHeap::top()
{
	return value[0];
}

void BinaryHeap::push(std::pair<int, int> node)
{
	value.push_back(node);
	siftUp(value.size() - 1);
}

void BinaryHeap::pop()
{
	value[0] = value[value.size() - 1];
	value.pop_back();
	heapify(0);
}

void BinaryHeap::siftUp(int index)
{
	while (parent(index) >= 0 && value[index] < value[parent(index)])
	{
		std::pair<int, int> parentValue = value[parent(index)];
		value[parent(index)] = value[index];
		value[index] = parentValue;
		index = parent(index);
	}
}

int BinaryHeap::parent(int index)
{
	return (index - 1) / 2;
}

void BinaryHeap::heapify(int index)
{
	int left = 2 * index + 1;
	int right = 2 * index + 2;
	int largest = index;
	if (left < value.size() && value[left] < value[largest])
	{
		largest = left;
	}
	if (right < value.size() && value[right] < value[largest])
	{
		largest = right;
	}
	if (largest != index)
	{
		std::pair<int, int> newLargest = value[largest];
		value[largest] = value[index];
		value[index] = newLargest;
		heapify(largest);
	}
}

bool BinaryHeap::empty()
{
	return value.empty();
}
