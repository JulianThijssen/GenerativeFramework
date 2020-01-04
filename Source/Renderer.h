#pragma once

#include "Camera.h"

#include <GDT/Shader.h>
#include <GDT/Matrix4f.h>
#include <GDT/Vector3f.h>
#include <GDT/OpenGL.h>

#include <vector>

struct Line
{
    Vector3f v0;
    Vector3f v1;
};

class Renderer
{
public:
    void init();
    void update();
    void drawLine(float x1, float y1, float x2, float y2);
    void drawLine(Vector3f v1, Vector3f v2);

    Camera& getCamera() { return camera; }

    Matrix4f modelMatrix;

private:
    Camera camera;

    GLuint _lineVao;
    GLuint _lineVbo;
    std::vector<Line> _lines;

    ShaderProgram _lineShader;
};
