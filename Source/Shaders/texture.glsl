const char* texture_frag = GLSL(330,
uniform sampler2D tex;

in vec2 pass_texCoord;

out vec4 fragColor;

void main()
{
    fragColor = texture(tex, pass_texCoord);
}
);
