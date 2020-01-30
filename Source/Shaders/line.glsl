#define GLSL(version, shader)  "#version " #version " core\n" #shader

const char* line_vert = GLSL(330,
    uniform mat4 projMatrix;
    uniform mat4 viewMatrix;
    uniform mat4 modelMatrix;

    uniform bool useColorAttribute;
    uniform vec3 colorUniform;

    layout(location = 0) in vec3 vertex;
    layout(location = 1) in vec4 position;
    layout(location = 2) in vec3 color;

    out vec3 passColor;

    void main()
    {
        if (useColorAttribute)
            passColor = color;
        else
            passColor = colorUniform;

        gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(vertex * position.w + position.xyz, 1);
    }
);

const char* line_frag = GLSL(330,
    in vec3 passColor;

    out vec4 fragColor;

    void main()
    {
        fragColor = vec4(passColor, 1.0);
    }
);
