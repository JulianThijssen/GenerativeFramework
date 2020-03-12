#pragma once

#include "Camera.h"
#include "PolyLine.h"
#include "Circle.h"

#include <GDT/Framebuffer.h>
#include <GDT/Texture.h>
#include <GDT/Shader.h>
#include <GDT/Matrix4f.h>
#include <GDT/Vector4f.h>
#include <GDT/Vector3f.h>
#include <GDT/Vector2f.h>
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
    void init(int width, int height);
    void update();
    void clear();
    void saveFramebufferContents(std::vector<unsigned char>& pixels);
    void drawLine(float x1, float y1, float x2, float y2);
    void drawLine(const Vector3f& v1, const Vector3f& v2);
    void drawPolyline(PolyLine polyLine);
    void drawCircle(Vector2f position, float radius, Vector3f color);

    Camera& getCamera() { return camera; }

    Matrix4f modelMatrix;

private:
    Camera camera;

    GLuint _unitQuadVao;
    GLuint _lineVao;
    GLuint _lineVbo;
    GLuint _circleVao;
    GLuint _circleVbo;
    GLuint _circlePropertyVbo;
    GLuint _circleColorVbo;
    std::vector<Line> _lines;
    std::vector<PolyLine> _polyLines;
    std::vector<Circle> _circles;

    ShaderProgram _lineShader;
    ShaderProgram _quadShader;

    Framebuffer _mainFramebuffer;
    Texture2D _mainTexture;
};
