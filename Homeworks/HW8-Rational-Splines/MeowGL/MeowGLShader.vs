R"(
#version 330 core

uniform mat4 ProjMtx;
in vec3 Position;

out vec4 Frag_Color;

void main()
{
    Frag_Color = vec4(0.5, 0.5, 0.5, 1);
    gl_Position = ProjMtx * vec4(Position.xyz, 1);
};
)"