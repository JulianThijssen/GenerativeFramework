#define GLSL(version, shader)  "#version " #version " core\n" #shader

const char* line_vert = GLSL(330,
    uniform mat4 projMatrix;
    uniform mat4 modelMatrix;

    layout(location = 0) in vec4 vertex;

    void main()
    {
        gl_Position = projMatrix * modelMatrix * vertex;
    }
);

const char* line_frag = GLSL(330,
    out vec4 fragColor;

    void main()
    {
        fragColor = vec4(1, 1, 1, 0.5);
    }
);
