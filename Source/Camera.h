#pragma once

#include <GDT/Vector3f.h>

class Camera
{
public:
    Vector3f position;
    Vector3f rotation;

    float fovy;
    float aspect;
    float zNear;
    float zFar;
};
