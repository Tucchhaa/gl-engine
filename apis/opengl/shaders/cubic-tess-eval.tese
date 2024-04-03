#version 330 core

uniform mat4 perspective;
uniform mat4 transform;
uniform mat3 normalTransform;

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

out vec2 texCoord;
out vec3 normal;
out vec3 fragPos;

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec4 U = vec4(u*u*u, u*u, u, 1);
    vec4 V = vec4(v*v*v, v*v, v, 1);

    float x = dot(U, B * Px[0] * BT * V);
    float y = dot(U, B * Py[0] * BT * V);
    float z = dot(U, B * Pz[0] * BT * V);

    gl_Position = perspective * transform * vec4(x, y, z, 1);
    fragPos = (transform * vec4(x, y, z, 1)).xyz;
    texCoord = gl_TessCoord.xy;

    vec4 dU = vec4(3*u*u, 2*u, 1, 0);
    vec4 dV = vec4(3*v*v, 2*v, 1, 0);
    vec3 dPdu = vec3(dot(dU, B * Px[0] * BT * V), dot(dU, B * Py[0] * BT * V), dot(dU, B * Pz[0] * BT * V));
    vec3 dPdv = vec3(dot(U, B * Px[0] * BT * dV), dot(U, B * Py[0] * BT * dV), dot(U, B * Pz[0] * BT * dV));
    normal = normalTransform * cross(dPdv, dPdu);
}
