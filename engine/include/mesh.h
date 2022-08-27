#pragma once

#include <glad/glad.h>

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