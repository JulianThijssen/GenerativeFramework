#include "Renderer.h"

#include <GDT/Math.h>

#define GLSL(version, shader)  "#version " #version " core\n" #shader

#include "Shaders/line.glsl"
#include "Shaders/quad.glsl"
#include "Shaders/texture.glsl"

void Renderer::init(int width, int height)
{
    // Set up main framebuffer
    _mainTexture.create();
    _mainTexture.bind(TextureUnit::TEXTURE0);
    _mainTexture.setData(width, height, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    _mainTexture.setSampling(NEAREST, NEAREST);
    _mainTexture.setWrapping(CLAMP, CLAMP);

    _mainFramebuffer.create();
    _mainFramebuffer.bind(Framebuffer::BOTH);
    _mainFramebuffer.addColorTexture(0, _mainTexture);
    _mainFramebuffer.validate();

    // Set up unit quad vao
    glGenVertexArrays(1, &_unitQuadVao);

    // Line VAO
    glGenVertexArrays(1, &_lineVao);
    glBindVertexArray(_lineVao);

    glGenBuffers(1, &_lineVbo);
    glBindBuffer(GL_ARRAY_BUFFER, _lineVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * _lines.size() * 2, _lines.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Circle instanced VAO
    std::vector<Vector3f> vertices;
    for (int seg = 0; seg < 32; seg++)
    {
        float angle = (seg / 32.0) * Math::TWO_PI;
        vertices.emplace_back(cos(angle), sin(angle), 0);
    }
    vertices.emplace_back(1, 0, 0);

    glGenVertexArrays(1, &_circleVao);
    glBindVertexArray(_circleVao);

    glGenBuffers(1, &_circleVbo);
    glBindBuffer(GL_ARRAY_BUFFER, _circleVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &_circlePropertyVbo);
    glBindBuffer(GL_ARRAY_BUFFER, _circlePropertyVbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STREAM_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);

    glGenBuffers(1, &_circleColorVbo);
    glBindBuffer(GL_ARRAY_BUFFER, _circleColorVbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STREAM_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    try {
        _lineShader.create();
        _lineShader.addShader(VERTEX, line_vert);
        _lineShader.addShader(FRAGMENT, line_frag);
        _lineShader.build();

        _quadShader.create();
        _quadShader.addShader(VERTEX, quad_vert);
        _quadShader.addShader(FRAGMENT, texture_frag);
        _quadShader.build();
    }
    catch (ShaderLoadingException e)
    {
        std::cout << e.what() << std::endl;
    }
}

void Renderer::update()
{
    _mainFramebuffer.bind();

    _lineShader.bind();

    Matrix4f projMatrix;
    float fovyr = camera.fovy * Math::PI / 180;

    projMatrix[0] = (float)(1 / tan(fovyr / 2)) / camera.aspect;
    projMatrix[5] = (float)(1 / tan(fovyr / 2));
    projMatrix[10] = (camera.zNear + camera.zFar) / (camera.zNear - camera.zFar);
    projMatrix[11] = -1;
    projMatrix[14] = (2 * camera.zNear * camera.zFar) / (camera.zNear - camera.zFar);
    projMatrix[15] = -0;

    Matrix4f viewMatrix;
    viewMatrix.rotate(-camera.rotation);
    viewMatrix.translate(-camera.position);

    _lineShader.uniformMatrix4f("projMatrix", projMatrix);
    _lineShader.uniformMatrix4f("viewMatrix", viewMatrix);
    _lineShader.uniformMatrix4f("modelMatrix", modelMatrix);

    glBindVertexArray(_lineVao);
    glBindBuffer(GL_ARRAY_BUFFER, _lineVbo);

    // Draw lines
    _lineShader.uniform1i("useColorAttribute", 0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Line) * _lines.size(), _lines.data(), GL_STREAM_DRAW);
    glDrawArrays(GL_LINES, 0, _lines.size() * 2);

    for (PolyLine& polyLine : _polyLines)
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * polyLine.getVertices().size(), polyLine.getVertices().data(), GL_STREAM_DRAW);
        glDrawArrays(GL_LINE_STRIP, 0, polyLine.getVertices().size());
    }

    _lineShader.uniform1i("useColorAttribute", 1);
    std::vector<Vector4f> circleProperties;
    std::vector<Vector3f> circleColors;
    for (Circle& circle : _circles)
    {
        circleProperties.emplace_back(circle.position.x, circle.position.y, 0, circle.radius);
        circleColors.push_back(circle.color);
    }
    glBindVertexArray(_circleVao);
    glBindBuffer(GL_ARRAY_BUFFER, _circlePropertyVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vector4f) * circleProperties.size(), circleProperties.data(), GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, _circleColorVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * circleColors.size(), circleColors.data(), GL_STREAM_DRAW);
    glDrawArraysInstanced(GL_LINE_STRIP, 0, 33, _circles.size());

    _lines.clear();
    _polyLines.clear();
    _circles.clear();

    _mainFramebuffer.release();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _quadShader.bind();
    glBindVertexArray(_unitQuadVao);

    _mainTexture.bind(TextureUnit::TEXTURE0);
    _quadShader.uniform1i("tex", 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Renderer::clear()
{
    _mainFramebuffer.bind();
    _mainFramebuffer.clearColorBuffer(0, 0.0f, 0.0f, 0.0f, 1.0f);
}

void Renderer::saveFramebufferContents(std::vector<unsigned char>& pixels)
{
    pixels.resize(_mainTexture.getWidth() * _mainTexture.getHeight() * 4);
    _mainTexture.bind(TextureUnit::TEXTURE0);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
}

void Renderer::drawPolyline(PolyLine polyLine)
{
    _polyLines.push_back(polyLine);
}

void Renderer::drawLine(float x1, float y1, float x2, float y2)
{
    _lines.push_back(Line{ Vector3f(x1, y1, 0), Vector3f(x2, y2, 0) });
}

void Renderer::drawLine(const Vector3f& v1, const Vector3f& v2)
{
    _lines.push_back(Line{ v1, v2 });
    //startIndex = _lines.size() > 9999 ? _lines.size() - 10000 : 0;
}

void Renderer::drawCircle(Vector2f position, float radius, Vector3f color = Vector3f(1, 1, 1))
{
    _circles.push_back(Circle{ position, radius, color });
}
