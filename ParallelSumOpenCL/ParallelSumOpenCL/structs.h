#ifndef CL_STRUCTS_H_
#define CL_STRUCTS_H_

#define MAX_STACK 1500
    
typedef struct{
	float A[3];
	float B[3];
	float C[3];
	float normal[3];
	int materialId;
} Triangle;

typedef struct{
	float position[3];
	int materialIndex;
} Light;

typedef struct{

	float amb[3];
	float diff[3];
	float spec[3];
	float shiny;
	float reflect;
	float transparency;
	float glossy;
} Material;

typedef struct{
	float3 position;
	float3 u;
	float3 v;
	float3 w;

} Camera;

typedef struct{
	float position[3];
	float normal[3];
	float t;
	int materialId;
} HitPoint;

typedef struct{
	float3 origin;
	float3 direction;
	float3 inverseDir;
} Ray;

typedef struct{
	float minBoundaries[3];
	float maxBoundaries[3];
} BoundingBox;

typedef struct{
	BoundingBox bBox;
	int left;
	int right;
	// If it's leaf, then the value is 1.
	int isLeaf;
} Node;

typedef struct{
    int indices[MAX_STACK];
    int top;
} NodeStack;

#endif