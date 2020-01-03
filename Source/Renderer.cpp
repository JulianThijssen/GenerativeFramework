#include "Renderer.h"

#include <GDT/Matrix4f.h>
#include <GDT/Math.h>

#include "Shaders/line.glsl"

void Renderer::init()
{
    glGenVertexArrays(1, &_lineVao);
    glBindVertexArray(_lineVao);

    glGenBuffers(1, &_lineVbo);
    glBindBuffer(GL_ARRAY_BUFFER, _lineVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * _lines.size() * 2, _lines.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    try {
        _lineShader.create();
        _lineShader.addShader(VERTEX, line_vert);
        _lineShader.addShader(FRAGMENT, line_frag);
        _lineShader.build();
    }
    catch (ShaderLoadingException e)
    {
        std::cout << e.what() << std::endl;
    }
}
float rot = 0;
int startIndex = 0;
void Renderer::update()
{
    glBindBuffer(GL_ARRAY_BUFFER, _lineVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * (_lines.size()-startIndex) * 2, _lines.data() + startIndex, GL_STREAM_DRAW);

    _lineShader.bind();

    Matrix4f projMatrix;
    float fovyr = 60 * Math::PI / 180;
    float aspect = 1;
    float zNear = 0.1f;
    float zFar = 40.0f;
    projMatrix[0] = (float)(1 / tan(fovyr / 2)) / aspect;
    projMatrix[5] = (float)(1 / tan(fovyr / 2));
    projMatrix[10] = (zNear + zFar) / (zNear - zFar);
    projMatrix[11] = -1;
    projMatrix[14] = (2 * zNear * zFar) / (zNear - zFar);
    projMatrix[15] = -0;

    Matrix4f modelMatrix;
    rot += 0.5f;
    modelMatrix.translate(Vector3f(0, 0, -10));
    modelMatrix.rotate(rot, 0, 1, 0);

    _lineShader.uniformMatrix4f("projMatrix", projMatrix);
    _lineShader.uniformMatrix4f("modelMatrix", modelMatrix);

    glBindVertexArray(_lineVao);

    glDrawArrays(GL_LINES, 0, _lines.size() * 2);
    //std::cout << _lines.size() << std::endl;
}

void Renderer::drawLine(float x1, float y1, float x2, float y2)
{
    _lines.push_back(Line{ Vector3f(x1, y1, 0), Vector3f(x2, y2, 0) });
}

void Renderer::drawLine(Vector3f v1, Vector3f v2)
{
    _lines.push_back(Line{ Vector3f(v1.x, v1.y, v1.z), Vector3f(v2.x, v2.y, v2.z) });
    startIndex = _lines.size() > 9999 ? _lines.size() - 10000 : 0;
}
