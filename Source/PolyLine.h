#pragma once

#include <GDT/Vector3f.h>

#include <vector>

class PolyLine
{
public:
    void addVertex(Vector3f v)
    {
        vertices.push_back(v);
    }

    const std::vector<Vector3f>& getVertices()
    {
        return vertices;
    }

private:
    std::vector<Vector3f> vertices;
};
