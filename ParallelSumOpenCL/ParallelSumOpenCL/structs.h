typedef struct{
	float A[3];
	float B[3];
	float C[3];
	float normal[3];
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
	float3 position;
	float3 u;
	float3 v;
	float3 w;

} Camera;

typedef struct{
	float position[3];
	float t;
	int triangleID;
} HitPoint;

typedef struct{
	float3 origin;
	float3 direction;
	float3 inverseDir;
} Ray;