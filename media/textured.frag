#version 450 core
out vec4 FragColor;

in vec3 Color;
in vec2 TexCoord;

uniform sampler2D sTexture;

void main() {
       //vec4(1.0f, 0.435f, 0.412f, 1.0f);
    FragColor = texture(sTexture, TexCoord) * vec4(Color, 1.0f);
}
