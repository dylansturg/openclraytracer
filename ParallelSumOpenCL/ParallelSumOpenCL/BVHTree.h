#ifndef BVH_TREE_
#define BVH_TREE_

#include "BVHNode.h"
#include <vector>
#include "Triangle.h"
#include "CPURayTracer\Rays\Ray.h"


using namespace std;

class BVHTree
{
public:
	BVHTree(vector<Triangle> & triangleList)
	{		
		this->nodesList = new vector<BVHNode>();
		nodesList->push_back(BVHNode(triangleList, nodesList, 0, triangleList.size()));
	}

	BVHTree(){

	}

	vector<BVHNode>* getNodesList(){
		return this->nodesList;
	}

	void intersect(Ray& ray){
		(*this->nodesList)[this->nodesList->size() - 1].intersect(ray);
	}

private:
	vector<BVHNode>* nodesList;
};


#endif