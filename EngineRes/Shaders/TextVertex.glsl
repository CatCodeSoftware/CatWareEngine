#version 460 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoords;
out vec2 v_TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = vec4(position.xy, 0.0, 1.0);
    v_TexCoords = texCoords;
}  