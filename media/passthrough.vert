#version 450 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec2 vTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Color;
out vec2 TexCoord;

void main() {
    gl_Position = projection * view * model * vec4(vPosition, 1.0);
    Color = vColor;
    TexCoord = vTexCoord;
}
