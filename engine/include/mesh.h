#pragma once

#include "platform.h"

#include <vector>
#include <initializer_list>

#ifndef PLATFORM_WEB
#include <glad/glad.h>
#else
#include <GLES3/gl3.h>
#endif

#define VBO_COUNT 3

struct VertexBufferLayout {
    GLenum dataType;
    int memberCount;
    GLuint dataLength;
    void* data;
};

class Mesh {
private: 
    GLuint indicesBuffer;
    std::vector<GLuint> vbos;

    GLuint vertexCount;
    GLuint indexCount;

    static int getDataSize(GLenum dataType);
public:
    Mesh(const GLuint indices_length, const unsigned int* indices, const GLuint vertices_length, const float* vertices, const GLuint uvs_length, const float* uvs);
    ~Mesh();

    GLuint attachBuffer(GLenum dataType, int memberCount, GLuint dataLength, void* data);

    GLuint handle;
    void bind();
    void unbind();

    GLuint getVertexCount() { return vertexCount; }
    GLuint getIndexCount() { return indexCount;  }
};
