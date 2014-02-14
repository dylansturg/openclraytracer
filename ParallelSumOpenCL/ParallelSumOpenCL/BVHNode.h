#ifndef BVH_NODE_
#define BVH_NODE_

#include <vector>
#include <algorithm>
#include <utility>
#include "Triangle.h"
#include "BoundingBox.h"
#include "comparator.h"
#include "CPURayTracer\Rays\Ray.h"


using namespace std;


class BVHNode{
public:
	BoundingBox bBox;
	int left;
	int right;
	// If it's leaf, then the value is 1.
	int isLeaf;
	/*vector<BVHNode>* nodeList;
	vector<Triangle>* triangles;
*/
	BVHNode(vector<Triangle> & triangleList, vector<BVHNode>* nodeLists, int left, int right)
	{
		//this->nodeList = nodeLists;
		//this->triangles = &triangleList;
		bBox = BoundingBox(&triangleList, left, right);
		this->isLeaf = 0;

		this->right = -1;
		
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

		nodeLists->push_back(BVHNode(triangleList, nodeLists, left, patitionIndex));
		this->left = nodeLists->size() - 1;
		nodeLists->push_back(BVHNode(triangleList, nodeLists, patitionIndex, right));
		this->right = nodeLists->size() - 1;


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

	void intersect(Ray& ray)
	{
		if (this->bBox.intersect(ray))
		{
			if (!this->isLeaf)
			{
				//this->nodeList->at(left).intersect(ray);
				if (this->right > 0)
				{
					//this->nodeList->at(right).intersect(ray);
				}
			}
			else 
			{
				//this->triangles->at(left).intersect(ray);
				if (this->right >= 0){
					//this->triangles->at(right).intersect(ray);
				}
			}
		}
	}

	
private:

};

#endif