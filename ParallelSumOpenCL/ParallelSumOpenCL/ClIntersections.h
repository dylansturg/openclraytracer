int intersect( HitPoint* hitPoint, Ray * ray, __global Triangle * triangle);
    
int intersect(HitPoint* hitPoint, Ray * ray, __global Triangle * triangle){
	float a, b, c, d, e, f, g, h, i, j, k, l;
	float ei_minus_hf, gf_minus_di, dh_minus_eg;
	float ak_minus_jb, jc_minus_al, bl_minus_kc;
	float beta, gamma, M;

	float3 A, B, C;
    float3 D, E;
	A = (float3)(triangle->A[0], triangle->A[1], triangle->A[2]);
	B = (float3)(triangle->B[0], triangle->B[1], triangle->B[2]);
	C = (float3)(triangle->C[0], triangle->C[1], triangle->C[2]);
	D = ray->direction;
	E = ray->origin;

	a = A[0] - B[0];
	b = A[1] - B[1];
	c = A[2] - B[2];

	d = A[0] - C[0];
	e = A[1] - C[1];
	f = A[2] - C[2];

	g = D[0];
	h = D[1];
	i = D[2];

	j = A[0] - E[0];
	k = A[1] - E[1];
	l = A[2] - E[2];

	ei_minus_hf = e * i - h * f;
	gf_minus_di = g * f - d * i;
	dh_minus_eg = d * h - e * g;

	// start calculating interesting values
	M = a * ei_minus_hf + b * gf_minus_di + c * dh_minus_eg;

	if (M == 0) {
		return -1;
	}

	beta = (j * ei_minus_hf + k * gf_minus_di + l * dh_minus_eg) / M;

	if (beta < 0 || beta > 1) {
		// half plane test failed
		return -1;
	}

	ak_minus_jb = a * k - j * b;
	jc_minus_al = j * c - a * l;
	bl_minus_kc = b * l - k * c;

	gamma = (i * ak_minus_jb + h * jc_minus_al + g * bl_minus_kc) / M;

	if (gamma < 0 || gamma > 1) {
		// half plane test failed
		return -1;
	}

	// got a hit.. calculate the parameter

	float t = -(f * ak_minus_jb + e * jc_minus_al + d * bl_minus_kc) / M;
	if (t > 0 && beta + gamma <= 1) {
        if(hitPoint->t > t)
		{
            hitPoint->t = t;
            hitPoint->materialId = triangle->materialId;
            float3 pos = E + D*t;
            hitPoint->position[0] = pos.x;
            hitPoint->position[1] = pos.y;
            hitPoint->position[2] = pos.z;
            
            float3 norm = (float3) (triangle->normal[0], triangle->normal[1],triangle->normal[2]);
            normalize(norm);
            
			hitPoint->normal[0] = norm.x;
			hitPoint->normal[1] = norm.y;
			hitPoint->normal[2] = norm.z;
		    return 0;
       }
    }
	return -1;
}