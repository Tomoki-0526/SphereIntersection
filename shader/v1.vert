#version 330 core
layout(location = 0)in vec3 pos;
layout(location = 1)in vec3 nor;
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
out vec3 o_nor;

void main(void)
{
    gl_Position = ProjectionMatrix * ViewMatrix * vec4(pos, 1.0);
    vec4 tmp_nor = ProjectionMatrix * ViewMatrix * vec4(nor, 1.0);
    o_nor = normalize(tmp_nor).xyz;
    o_nor = nor;
}
