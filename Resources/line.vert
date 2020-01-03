#version 330 core

uniform mat4 projMatrix;
uniform mat4 modelMatrix;

layout(location = 0) in vec4 vertex;

void main()
{
    gl_Position = projMatrix * modelMatrix * vertex;
}
