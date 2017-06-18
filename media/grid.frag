#version 450 core
in vec2 TexCoord;
out vec4 FragColor;

void main() {
    vec3 gridColor = vec3(0.5f);
    vec2 grid = abs(fract(TexCoord - 0.5) - 0.5) / fwidth(TexCoord);
    float line = min(grid.x, grid.y);
    FragColor = vec4(gridColor, 1 - min(line, 1.0));
}
