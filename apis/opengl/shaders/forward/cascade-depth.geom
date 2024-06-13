#version 410 core

layout(triangles, invocations = 5) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 lightPerspectives[16];

void main() {
    for(int i = 0; i < 3; ++i) {
        gl_Position = lightPerspectives[gl_InvocationID] * gl_in[i].gl_Position;
        gl_Layer = gl_InvocationID;
        EmitVertex();
    }

    EndPrimitive();
}