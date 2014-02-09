#ifndef __GenVector
#define __GenVector

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//enable the padding conversion function - slow
//#define VECTOR_PADDING_CONVERSION

//control if all components are set to zero on construction
#define VECTOR_INITIALIZE_TO_ZERO

//default vector
#define v3float float

//error checking for divide by 0 and other degenerate cases
//#define VECTOR_ERROR_CHECKING

// set an epsilon if not set
#ifndef EPSILON
#define EPSILON 0.000000001f
#endif

template <int dimension, class T = v3float, int padding = 0>
class GenVector
{
public:

	static const int dim = dimension;
	T c[dimension+padding]; //typically 3 dim of x, y, z
	
	// GenVector creation
	GenVector()
	{
		#ifdef VECTOR_INITIALIZE_TO_ZERO
		for(int i=0; i<dimension; i++)
			c[i] = 0;
		#endif
	}
	
	GenVector(const GenVector &source)
	{
		for(int i=0; i<dimension; i++)
			c[i] = source.c[i];
	}
	
	GenVector(const T *source)
	{
		for(int i=0; i<dimension; i++)
			c[i] = source[i];
	}
	
	GenVector(const T f)
	{
		for(int i=0; i<dimension; i++)
			c[i] = f;
	}

	GenVector(const T x, const T y)
	{ c[0] = x; c[1] = y; }
	
	GenVector(const T x, const T y, const T z)
	{ c[0] = x; c[1] = y; c[2] = z; }
	
	GenVector(const T x, const T y, const T z, const T w)
	{ c[0] = x; c[1] = y; c[2] = z; c[3] = w; }
	
	GenVector(const GenVector &to, const GenVector &from)
	{
		for(int i=0; i<dimension; i++)
			c[i] = to.c[i] - from.c[i];
	}
	
	#ifdef VECTOR_PADDING_CONVERSION
	template <int paddingIn>
	operator GenVector<dimension, T, paddingIn>() const
	{
		GenVector<dimension, T, paddingIn> out;
		for(int i=0; i<dimension; i++)
			out.c[i] = this->c[i];
		return out;
	}
	#endif
	
	// operator overload
	GenVector operator+(const GenVector &v1) const
	{
		GenVector t;
		for(int i=0; i<dimension; i++)
			t.c[i] = c[i] + v1.c[i];
		return t;
	}
	
	GenVector operator-(const GenVector &v1) const
	{
		GenVector t;
		for(int i=0; i<dimension; i++)
			t.c[i] = c[i] - v1.c[i];
		return t;
	}
	
	GenVector operator*(const GenVector &v1) const
	{
		GenVector t;
		for(int i=0; i<dimension; i++)
			t.c[i] = c[i] * v1.c[i];
		return t;
	}
	
	GenVector operator/(const GenVector &v1) const
	{
		GenVector t;
		for(int i=0; i<dimension; i++)
			t.c[i] = c[i] / v1.c[i];
		return t;
	}
	
	GenVector operator+(const T f) const
	{
		GenVector t;
		for(int i=0; i<dimension; i++)
			t.c[i] = c[i] + f;
		return t;
	}
	
	GenVector operator-(const T f) const
	{
		GenVector t;
		for(int i=0; i<dimension; i++)
			t.c[i] = c[i] - f;
		return t;
	}
	
	GenVector operator*(const T f) const
	{
		GenVector t;
		for(int i=0; i<dimension; i++)
			t.c[i] = c[i] * f;
		return t;
	}
	
	GenVector operator/(const T f) const
	{
		GenVector t;
		for(int i=0; i<dimension; i++)
			t.c[i] = c[i] / f;
		return t;
	}
	
	GenVector& operator+=(const GenVector &v1)
	{
		for(int i=0; i<dimension; i++)
			c[i] += v1.c[i];
		return *this;
	}
	
	GenVector& operator-=(const GenVector &v1)
	{
		for(int i=0; i<dimension; i++)
			c[i] -= v1.c[i];
		return *this;
	}
	
	GenVector& operator*=(const GenVector &v1)
	{
		for(int i=0; i<dimension; i++)
			c[i] *= v1.c[i];
		return *this;
	}
	
	GenVector& operator/=(const GenVector &v1)
	{
		for(int i=0; i<dimension; i++)
			c[i] /= v1.c[i];
		return *this;
	}
	
	GenVector& operator+=(const T f)
	{
		for(int i=0; i<dimension; i++)
			c[i] += f;
		return *this;
	}
	
	GenVector& operator-=(const T f)
	{
		for(int i=0; i<dimension; i++)
			c[i] -= f;
		return *this;
	}
	
	GenVector& operator*=(const T f)
	{
		for(int i=0; i<dimension; i++)
			c[i] *= f;
		return *this;
	}
	
	GenVector& operator/=(const T f)
	{
		for(int i=0; i<dimension; i++)
			c[i] /= f;
		return *this;
	}
	
	GenVector& operator=(const GenVector &v1)
	{
		for(int i=0; i<dimension; i++)
			c[i] = v1.c[i];
		return *this;
	}
	
	template<int otherDim>
	GenVector& operator=(const GenVector<otherDim> &v1)
 	{
		#ifdef VECTOR_ERROR_CHECKING
		if(otherDim < dimension)
		{
			throw(1);
		}
		#endif

		//truncate extra dimension in input vector
 		for(int i=0; i<dimension; i++)
 			c[i] = v1.c[i];
 		return *this;
	}
	
	GenVector& operator=(const T f)
	{
		for(int i=0; i<dimension; i++)
			c[i] = f;
		return *this;
	}
	
	bool operator==(const GenVector &v1) const
	{
		bool equal = true;
		for(int i=0; i<dimension; i++)
			equal = equal && c[i] == v1.c[i];
		return equal;
	}
	
	bool operator!=(const GenVector &v1) const
	{
		return !(v1 == *this);
	}
	
	GenVector operator-() const
	{
		GenVector t;
		for(int i=0; i<dimension; i++)
			t.c[i] = -c[i];
		return t;
	}
	
	T operator[](const int i) const
	{
		return c[i];
	}
	
	T& operator[](const int i)
	{
		return c[i];
	}
	
	T operator()(const int i) const
	{
		return c[i];
	}
	
	T& operator()(const int i)
	{
		return c[i];
	}
	
	// arithmetic operations
	T dot(const GenVector &a) const
	{
		T f = 0;
		for(int i=0; i<dimension; i++)
			f += c[i]*a(i);
		return f;
		//return a.c[0]*c[0] + a.c[1]*c[1] + a.c[2]*c[2];
	}
	
	GenVector cross(const GenVector &a) const
	{
		#ifdef VECTOR_ERROR_CHECKING
		if(dimension!=3)
		{
			//throw exception? explode?
			return (*this);
		}
		#endif
	
		GenVector dest;
		dest.c[0] = c[1]*a.c[2] - c[2]*a.c[1];
		dest.c[1] = c[2]*a.c[0] - c[0]*a.c[2];
		dest.c[2] = c[0]*a.c[1] - c[1]*a.c[0];
		return dest;
	}
	
	// GenVector unit operations
	T squaredLength() const
	{
		T sqrdLength = 0.0f;
		for(int i=0; i<dimension; i++)
			sqrdLength += c[i] * c[i];
		return sqrdLength;
	}
	
	T length() const
	{
		return sqrt(this->squaredLength());
	}
	
	GenVector& normalize()
	{
		T normalizeLength;
		normalizeLength = this->length();
		
		#ifdef VECTOR_ERROR_CHECKING
		if(normalizeLength <= EPSILON)
		{
			throw(1);
			return *this;
		}
		#endif
		
		*this /= normalizeLength;
		return *this;
	}
	
	// GenVector combination operations
	T distanceSquared(const GenVector &a)
	{
		T d = 0.0;
		for(int i=0; i<dimension; i++)
			d += pow(a.c[i] - c[i], 2);
		return d;
	}

	T distance(const GenVector& a)
	{
		T disSquared = this->lengthSquared(a);
		return sqrt( disSquared );
	}
	
	T angleBetweem(const GenVector &a)
	{
		return acos(this->dot(a) / this->length() / a.length());
	}
	
	GenVector reflect(const GenVector &normal)
	{
		T dp;
		GenVector dest;
		
		dp = 2*this->dot(normal);
		dest = *this - (normal*dp);
		return dest;
	}
	
	void projectToPlane(const GenVector &normal)
	{
		float scale = this->dot(normal);
		*this = *this - (normal * scale);
	}

	int maxComponent()
	{
		int largest = 0;
		for(int i=1; i<dimension; i++)
			if(c[i] > c[i-1])
				largest = i;
		return largest;
	}
	
	int maxMagnitudeComponent()
	{
		int largest = 0;
		for(int i=1; i<dimension; i++)
			if(fabs(c[i]) > fabs(c[i-1]))
				largest = i;
		return largest;
	}
};

template <int dimension, class T, int padding>
static GenVector<dimension, T, padding> operator*(const T f, const GenVector<dimension, T, padding> &v1)
{
	return v1*f;
}

template <int dimension, class T, int padding>
static GenVector<dimension, T, padding> operator-(const T f, const GenVector<dimension, T, padding> &v1)
{
	GenVector<dimension, T, padding> t;
	for(int i=0; i<v1.dim; i++)
		t[i] = f - v1[i];
	return t;
}

template <int dimension, class T, int padding>
static GenVector<dimension, T, padding> operator/(const T f, const GenVector<dimension, T, padding> &v1)
{
	GenVector<dimension, T, padding> t;
	for(int i=0; i<v1.dim; i++)
		t[i] = f / v1[i];
	return t;
}

typedef GenVector<2> Vector2;
typedef GenVector<3> Vector3;
typedef GenVector<4> Vector4;
typedef GenVector<3, unsigned char> Color;

#endif
