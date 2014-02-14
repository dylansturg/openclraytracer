#ifndef BVH_NODE_
#define BVH_NODE_

#include <vector>
#include <algorithm>
#include <utility>
#include "Triangle.h"
#include "BoundingBox.h"
#include "comparator.h"


using namespace std;


class BVHNode{
public:
	BVHNode(vector<Triangle> & triangleList, vector<BVHNode> & nodeLists, int left, int right)
	{
		bBox = BoundingBox(&triangleList, left, right);
		this->isLeaf = 0;
		
		int range = right - left;

		if (range == 1){
			this->isLeaf = 1;
			this->left = left;
			return;
		}

		if (range == 2){
			this->isLeaf = 1;
			this->left = left;
			this->right = left+1;
			return;
		}

		pair<float, int> splitVal = bBox.getSplit();
		sort(triangleList.begin() + left, triangleList.begin() + right, comparators[splitVal.second]);
		
		int patitionIndex = findBoundary(&triangleList, left, right, splitVal.second, splitVal.first);

		if (patitionIndex <= left || patitionIndex >= right){
			patitionIndex = left + range / 2;
		}

		nodeLists.push_back(BVHNode(triangleList, nodeLists, left, patitionIndex));
		this->left = nodeLists.size() - 1;
		nodeLists.push_back(BVHNode(triangleList, nodeLists, patitionIndex, right));
		this->right = nodeLists.size() - 1;


	}

	int findBoundary(vector<Triangle> *surfaces, int start, int end, int axis,
		float midValue)
	{
		for (int i = start; i < end; i++)
		{
			if ((*surfaces)[i].getCenter()[axis] > midValue)
			{
				return i;
			}
		}
		return start;
	}

	
private:
	BoundingBox bBox;
	int left;
	int right;
	// If it's leaf, then the value is 1.
	int isLeaf;

};

#endif