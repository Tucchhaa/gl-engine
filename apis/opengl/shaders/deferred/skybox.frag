#version 410 core

uniform samplerCube skybox;

in vec3 texCoords;

out vec4 color;

void main()
{
    color = texture(skybox, texCoords);
}
