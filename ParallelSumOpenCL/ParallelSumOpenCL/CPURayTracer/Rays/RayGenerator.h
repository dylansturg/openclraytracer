#pragma once

#include "../../GenVector.h"
#include "../../Camera.h"
#include "Ray.h"
#include <vector>

using namespace std;

class RayGenerator
{
public:
    RayGenerator(Camera c, unsigned int w, unsigned int h)
    {
        this->width = (float)w;
        this->height = (float)h;
        this->imageDistance = (float)(h >> 1);
        this->camera = c;

    }

    ~RayGenerator()
    {

    }

    Ray getRayAt(unsigned int x, unsigned int y)
    {
		Vector3 rayOrigin = Vector3(this->camera.Pos.s[0], this->camera.Pos.s[1], this->camera.Pos.s[2]);
        // float imageDist = (float)(this->height >> 1); //calculateDistToImagePlane(this->camera.getFOV());

        float l, b, w, h;

        w = (float)this->width;
        h = (float)this->height;

        l = -((float)(this->width >> 1));
        b = -((float)(this->height >> 1));

        float uPos = l + (w * ((float)x + 0.5)) / w;
        float vPos = b + (h * ((float)y + 0.5)) / h;
		Vector3 camW = Vector3(this->camera.w.s[0], this->camera.w.s[1], this->camera.w.s[2]);
		Vector3 camU = Vector3(this->camera.u.s[0], this->camera.u.s[1], this->camera.u.s[2]);
		Vector3 camV = Vector3(this->camera.v.s[0], this->camera.v.s[1], this->camera.v.s[2]);
        Vector3 rayDir = (-this->imageDistance * camW) + (uPos * camU) + (vPos * camV);
        return Ray(rayOrigin, rayDir);
    }

private:
    vector<Ray> rays;
    unsigned int width;
    unsigned int height;
    Camera camera;
    float imageDistance;

    float calculateDistToImagePlane(float fov)
    {
        // for now, hard code to 90 degree fov... maybe fix later
        return ((float)this->height) / 2;
    }


};