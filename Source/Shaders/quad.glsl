const char* quad_vert = GLSL(330,

vec3 vertices[4] = vec3[](
    vec3(-1, -1, 0),
    vec3(1, -1, 0),
    vec3(-1, 1, 0),
    vec3(1, 1, 0)
);

vec2 texCoords[4] = vec2[](
    vec2(0, 0),
    vec2(1, 0),
    vec2(0, 1),
    vec2(1, 1)
);

out vec3 pass_position;
out vec2 pass_texCoord;

void main() {
    pass_position = vertices[gl_VertexID];
    pass_texCoord = texCoords[gl_VertexID];
    
    gl_Position = vec4(vertices[gl_VertexID], 1);
}
);
