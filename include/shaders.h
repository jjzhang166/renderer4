#pragma once

const char *vertShaderSrc =
    "#version 450 core                                               \n"
    "layout (location = 0) in vec3 position;                         "
    "void main() {                                                   "
    "    gl_Position = vec4(position.x, position.y, position.z, 1.0);"
    "}                                                               ";

const char *fragShaderSrc =
    "#version 450 core                                               \n"
    "out vec4 FragColor;                                             "
    "void main() {                                                   "
    "    FragColor = vec4(1.0f, 0.435f, 0.412f, 1.0f);                   "
    "}                                                               ";
