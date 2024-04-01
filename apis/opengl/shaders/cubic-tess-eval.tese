#version 330 core

uniform mat4 perspective;

layout (quads, fractional_odd_spacing, ccw) in;

const mat4 B = mat4(
    -1,  3, -3,  1,
     3, -6,  3,  0,
    -3,  3,  0,  0,
     1,  0,  0,  0
);

const mat4 BT = transpose(B);

in mat4 Px[];
in mat4 Py[];
in mat4 Pz[];

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec4 U = vec4(u*u*u, u*u, u, 1);
    vec4 V = vec4(v*v*v, v*v, v, 1);

    float x = dot(U, B * Px[0] * BT * V);
    float y = dot(U, B * Py[0] * BT * V);
    float z = dot(U, B * Pz[0] * BT * V);

    gl_Position = perspective * vec4(x, y, z, 1);
//    gl_Position = vec4(u, v, 0, 1);
}
