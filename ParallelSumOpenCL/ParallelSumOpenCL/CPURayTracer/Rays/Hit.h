#pragma once

#include "../../GenVector.h"

class Hit
{
public:
	Hit(){
		this->param = -1;
	}

    Hit(float p)
    {
        this->param = p;
    }

    Hit(float p, int matIndex, Vector3 norm)
    {
        this->param = p;
        this->hitMaterialId = matIndex;
        this->normal = norm;
    }

    ~Hit()
    {

    }

    float getParam() const
    {
        return this->param;
    }

    void setParam(float t)
    {
        this->param = t;
    }

    int getHitMaterialId() const
    {
        return this->hitMaterialId;
    }
    Vector3 getHitNormal()
    {
        return this->normal;
    }

    bool operator<(const Hit &h1) const
    {
        return param < h1.getParam();
    }
    bool operator>(const Hit &h1) const
    {
        return param > h1.getParam();
    }
    bool operator==(const Hit &h1) const
    {
        return param == h1.getParam();
    }

private:
    float param;
    Vector3 normal;
    int hitMaterialId;
};
