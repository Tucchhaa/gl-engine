#version 410 core

layout (quads, fractional_odd_spacing, ccw) in;

const mat4 B = mat4(
    -1,  3, -3,  1,
     3, -6,  3,  0,
    -3,  3,  0,  0,
     1,  0,  0,  0
);

const mat4 BT = transpose(B);

uniform mat4 perspective;
uniform mat4 lightPerspective;
uniform mat4 transform;
uniform mat3 normalTransform;

in mat4 Px[];
in mat4 Py[];
in mat4 Pz[];

out vec2 texCoord;
out vec3 normal;
out vec3 fragPos;
out vec4 fragPosLightSpace;
out mat3 TBN;

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec4 U = vec4(u*u*u, u*u, u, 1);
    vec4 V = vec4(v*v*v, v*v, v, 1);

    mat4 BPxBT = B * Px[0] * BT;
    mat4 BPyBT = B * Py[0] * BT;
    mat4 BPzBT = B * Pz[0] * BT;

    float x = dot(U, BPxBT * V);
    float y = dot(U, BPyBT * V);
    float z = dot(U, BPzBT * V);

    vec4 worldPos = transform * vec4(x, y, z, 1);

    // === Partial derivatives ===
    vec4 dU = vec4(3*u*u, 2*u, 1, 0);
    vec4 dV = vec4(3*v*v, 2*v, 1, 0);
    vec3 dPdu = vec3(dot(dU, BPxBT * V), dot(dU, BPyBT * V), dot(dU, BPzBT * V));
    vec3 dPdv = vec3(dot(U, BPxBT * dV), dot(U, BPyBT * dV), dot(U, BPzBT * dV));

    gl_Position = perspective * worldPos;
    fragPos = worldPos.xyz;
    fragPosLightSpace = lightPerspective * worldPos;
    texCoord = gl_TessCoord.xy;
    normal = normalTransform * cross(dPdv, dPdu);

    TBN = transpose(mat3(
        normalize(vec3(transform * vec4(dPdu, 0.0f))),
        normalize(vec3(transform * vec4(dPdv, 0.0f))),
        normalize(vec3(transform * vec4(normal, 0.0f)))
    ));


}
