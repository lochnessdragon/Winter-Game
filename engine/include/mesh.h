#pragma once

#include "platform.h"

#ifndef PLATFORM_WEB
#include <glad/glad.h>
#else
#include <GLES3/gl3.h>
#endif

class Mesh {
private: 
    GLuint handle;
    GLuint vbos[2];
public:
    Mesh(const GLuint vertices_length, const float* vertices, const GLuint indices_length, const int* indices);
    ~Mesh();

    void bind();
    void unbind();
};