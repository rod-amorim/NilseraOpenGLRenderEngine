#version 330 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 aTexCoord;

out vec4 ourColor;
out vec2 texCoord;
out vec3 normal;
out vec3 fragPos;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_view * u_model * aPos;
    fragPos = vec3(u_model * aPos);
    texCoord = aTexCoord;

    //Study porposes, fix the light in cases wherer there is a non uniform scaling, this should be done in the cpu side and passed as uniform
    normal = mat3(transpose(inverse(u_model))) * vertexNormal;
};