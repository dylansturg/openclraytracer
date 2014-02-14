#ifndef BVH_TREE_
#define BVH_TREE_

#include "BVHNode.h"
#include <vector>
#include "Triangle.h"


using namespace std;

class BVHTree
{
public:
	BVHTree(vector<Triangle> & triangleList)
	{		
		nodesList.push_back(BVHNode(triangleList, nodesList, 0, triangleList.size()));
	}

	vector<BVHNode> & getNodesList(){
		return this->nodesList;
	}

private:
	vector<BVHNode> nodesList;
};


#endif