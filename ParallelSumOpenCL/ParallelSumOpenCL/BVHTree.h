#ifndef BVH_TREE_
#define BVH_TREE_

#include "BVHNode.h"
#include <vector>
#include "Triangle.h"
#include "CPURayTracer\Rays\Ray.h"


typedef struct{
	int indices[1500];
	int top;
} NodeStack;

void stackPush(NodeStack* stack, int value)
{
	if (stack->top < 1500)
	{
		stack->indices[stack->top] = value;
		stack->top++;
	}
}

int stackPop(NodeStack* stack)
{
	if (stack->top >= 0){
		int val = stack->indices[stack->top-1];
		stack->top--;
		return val;
	}
	return -1;
}

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

	int intersectTree(Ray* ray, vector<Triangle> triangles)
	{
		NodeStack stack;
		int triangleSize = 33;
		
		stack.top = 0;
		stackPush(&stack, this->nodesList->size() - 1);

		while (stack.top > 0)
		{
			int curIndex = stackPop(&stack);
			if (curIndex < 0){
				return -1;
			}
			BVHNode curNode = (*this->nodesList)[curIndex];

			BoundingBox bBox = curNode.bBox;

			if (bBox.intersect(*ray))
			{
				if (curNode.isLeaf != 1)
				{
					stackPush(&stack, curNode.left);
					if (curNode.right > 0)
					{
						stackPush(&stack, curNode.right);
					}
				}
				else
				{

					triangles[curNode.left].intersect(*ray);

					if (curNode.right >= 0){

						triangles[curNode.right].intersect(*ray);

					}
				}
			}
		}
		return 0;
	}

private:
	vector<BVHNode>* nodesList;
};


#endif