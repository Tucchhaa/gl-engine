#version 330 core

uniform float tesselationLevel;

layout (vertices=16) out;

out mat4 Px[];
out mat4 Py[];
out mat4 Pz[];

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    if(gl_InvocationID == 0)
    {
        gl_TessLevelOuter[0] = tesselationLevel;
        gl_TessLevelOuter[1] = tesselationLevel;
        gl_TessLevelOuter[2] = tesselationLevel;
        gl_TessLevelOuter[3] = tesselationLevel;

        gl_TessLevelInner[0] = tesselationLevel;
        gl_TessLevelInner[1] = tesselationLevel;

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
