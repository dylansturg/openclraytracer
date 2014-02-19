#pragma once
#ifndef __Triangle
#define __Triangle

#include "stdafx.h"
#include <utility>
#include <CL/cl.h>

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include "taco.h"
#include "GenVector.h"
#include "CPURayTracer\Rays\Ray.h"

class Triangle{
public:
	cl_float a[3];
	cl_float b[3];
	cl_float c[3];
	cl_float normal[3];
	cl_int materialIndex;

	Triangle(){}

	Triangle(cl_float3 a, cl_float3 b, cl_float3 c, cl_int materialIndex, cl_float3 normal){
		for (int i = 0; i < 3; i++){
			this->a[i] = a.s[i];
			this->b[i] = b.s[i];
			this->c[i] = c.s[i];
		}
		this->materialIndex = materialIndex;
		calculateNormal();
	}

	Vector3 getMinBoundaries(){
		float minX =
			a[0] < b[0] ?
			(a[0] < c[0] ?
			a[0] : c[0]) :
			(b[0] < c[0] ?
			b[0] : c[0]);
		float minY =
			a[1] < b[1] ?
			(a[1] < c[1] ?
			a[1] : c[1]) :
			(b[1] < c[1] ?
			b[1] : c[1]);
		float minZ =
			a[2] < b[2] ?
			(a[2] < c[2] ?
			a[2] : c[2]) :
			(b[2] < c[2] ?
			b[2] : c[2]);
		return Vector3(minX, minY, minZ);
	}

	Vector3 getMaxBoundaries(){
		float maxX =
			a[0] > b[0] ?
			(a[0] > c[0] ?
			a[0] : c[0]) :
			(b[0] > c[0] ?
			b[0] : c[0]);
		float maxY =
			a[1] > b[1] ?
			(a[1] > c[1] ?
			a[1] : c[1]) :
			(b[1] > c[1] ?
			b[1] : c[1]);

		float maxZ =
			a[2] > b[2] ?
			(a[2] > c[2] ?
			a[2] : c[2]) :
			(b[2] > c[2] ?
			b[2] : c[2]);
		return Vector3(maxX, maxY, maxZ);
	}

	Vector3 getCenter()
	{
		return Vector3((a[0] + b[0] + c[0]) / 3, (a[1] + b[1] + c[1]) / 3, (a[2] + b[2] + c[2]) / 3);
	}

	bool intersect(Ray& ray)
	{
		float a, b, c, d, e, f, g, h, i, j, k, l;
		float ei_minus_hf, gf_minus_di, dh_minus_eg;
		float ak_minus_jb, jc_minus_al, bl_minus_kc;
		float beta, gamma, M;

		Vector3 A, B, C, D, E;
		A = Vector3(this->a[0], this->a[1], this->a[2]);
		B = Vector3(this->b[0], this->b[1], this->b[2]);
		C = Vector3(this->c[0], this->c[1], this->c[2]);
		D = ray.getDirection();
		E = ray.getOrigin();

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
			return false;
		}

		beta = (j * ei_minus_hf + k * gf_minus_di + l * dh_minus_eg) / M;

		if (beta < 0 || beta > 1) {
			// half plane test failed
			return false;
		}

		ak_minus_jb = a * k - j * b;
		jc_minus_al = j * c - a * l;
		bl_minus_kc = b * l - k * c;

		gamma = (i * ak_minus_jb + h * jc_minus_al + g * bl_minus_kc) / M;

		if (gamma < 0 || gamma > 1) {
			// half plane test failed
			return false;
		}

		// got a hit.. calculate the parameter

		float t = -(f * ak_minus_jb + e * jc_minus_al + d * bl_minus_kc) / M;
		if (t > 0 && beta + gamma <= 1) {
			ray.addHit(Hit(t, 0, Vector3(1, 1, 1)));
			return true;
		}
		else {
			return false;
		}

	}

private:

	void calculateNormal(){
		Vector3 aTemp;
		Vector3 bTemp;
		Vector3 cTemp;
		for (int i = 0; i < 3; i++){
			aTemp.c[i] = a[i];
			bTemp.c[i] = b[i];
			cTemp.c[i] = c[i];
		}

		Vector3 n = (bTemp - aTemp).cross(cTemp - bTemp).normalize();

		for (int i = 0; i < 3; i++){
			this->normal[i] = n.c[i];
		}
	}

};
#endif