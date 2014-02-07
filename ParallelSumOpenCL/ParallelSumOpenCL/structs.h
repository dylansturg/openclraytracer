typedef struct{
	float3 A;
	float3 B;
	float3 C;
	float3 normal;
	int materialIndex;
} Triangle;

typedef struct{
	float3 position;
	int materialIndex;
} Light;

typedef struct{

	float3 amb;
	float3 diff;
	float3 spec;
	float reflect;
	float shiny;
	float transparency;
	float glossy;
} Material;

typedef struct{
	float3 u;
	float3 v;
	float3 w;
	float3 position;
} Camera;

typedef struct{
	float3 position;
	float t;
	int triangleID;
} HitPoint;

typedef struct{
	float3 origin;
	float3 direction;
	float3 inverseDir;
} Ray;