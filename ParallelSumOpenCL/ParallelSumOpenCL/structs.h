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