#pragma once

#include "platform.h"

#ifndef PLATFORM_WEB
#include <glad/glad.h>
#else
#include <GLES3/gl3.h>
#endif

class Mesh {
private: 
    GLuint vbos[2];
public:
    Mesh(const GLuint vertices_length, const float* vertices, const GLuint indices_length, const int* indices);
    ~Mesh();

    GLuint handle;
    void bind();
    void unbind();
};