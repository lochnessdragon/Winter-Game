#pragma once

#include "platform.h"

#ifndef PLATFORM_WEB
#include <glad/glad.h>
#else
#include <GLES3/gl3.h>
#endif

#define VBO_COUNT 3

class Mesh {
private: 
    GLuint vbos[VBO_COUNT];
public:
    Mesh(const GLuint vertices_length, const float* vertices, const GLuint indices_length, const int* indices, const GLuint uvs_length, const float* uv_data);
    ~Mesh();

    GLuint handle;
    void bind();
    void unbind();
};