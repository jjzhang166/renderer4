//
// Created by adam on 6/17/17.
//

#ifndef RENDERER_TEXTURE_H
#define RENDERER_TEXTURE_H

#include <iostream>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

class Texture {
    GLuint  mTexture;
    GLint mWidth, mHeight, mChannels;

public:

    Texture(const std::string& filename) {
        glGenTextures(1, &mTexture);
        glBindTexture(GL_TEXTURE_2D, mTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_set_flip_vertically_on_load(true);
        auto image = stbi_load(filename.c_str(), &mWidth, &mHeight, &mChannels, 0);
        if(!image) {
            throw std::runtime_error("Failed to load " + filename);
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image);
    }

    ~Texture() {
        glDeleteTextures(1, &mTexture);
    }

    GLint getWidth() const {
        return mWidth;
    }

    GLint getHeight() const {
        return mHeight;
    }

    void bind() {
        glBindTexture(GL_TEXTURE_2D, mTexture);
    }

};

#endif //RENDERER_TEXTURE_H
