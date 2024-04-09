#version 410 core

uniform float tessInnerLevel;
uniform float tessOuterLevel;

layout (vertices=16) out;

out mat4 Px[];
out mat4 Py[];
out mat4 Pz[];

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    if(gl_InvocationID == 0)
    {
        gl_TessLevelOuter[0] = tessOuterLevel;
        gl_TessLevelOuter[1] = tessOuterLevel;
        gl_TessLevelOuter[2] = tessOuterLevel;
        gl_TessLevelOuter[3] = tessOuterLevel;

        gl_TessLevelInner[0] = tessInnerLevel;
        gl_TessLevelInner[1] = tessInnerLevel;

        // TODO: use gl_InvocationID to set the control points of the output patch
        for(int i = 0; i < 4; i++)
        {
            for(int j=0; j < 4; j++) {
                Px[0][i][j] = gl_in[i*4+j].gl_Position.x;
                Py[0][i][j] = gl_in[i*4+j].gl_Position.y;
                Pz[0][i][j] = gl_in[i*4+j].gl_Position.z;
            }
        }
    }
}
