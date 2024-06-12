
#version 410 core

layout (location = 0) in vec3 _position;

uniform mat4 rotationMatrix;

out vec3 texCoords;

void main()
{
    texCoords = _position;

    vec4 pos = rotationMatrix * vec4(_position, 1);

    gl_Position = pos.xyww;
}
