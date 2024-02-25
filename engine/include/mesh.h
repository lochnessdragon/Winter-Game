#pragma once

#include "platform.h"

#include <vector>
#include <initializer_list>

#ifndef PLATFORM_WEB
#include <glad/glad.h>
#else
#include <GLES3/gl3.h>
#endif

struct VertexBufferLayout {
    GLenum dataType;
    int memberCount;
    GLuint dataLength;
    void* data;
};

enum class MeshUsage {
    Static,
    Dynamic
};

class Mesh {
private: 
    GLuint indicesBuffer;
    std::vector<GLuint> vbos;

    GLuint vertexCount;
    GLuint indexCount;

    GLenum usage;

    static int getDataSize(GLenum dataType);
public:
    Mesh(MeshUsage usage);
    Mesh(const GLuint indices_length, const unsigned int* indices, const GLuint vertices_length, const float* vertices, const GLuint uvs_length, const float* uvs, MeshUsage usage = MeshUsage::Static);
    ~Mesh();

    GLuint attachBuffer(GLenum dataType, int memberCount, GLuint dataLength, void* data);
    void setIndexBuffer(const GLuint indices_length, const unsigned int* indices);
    void setBuffer(uint32_t bufferIdx, const GLuint bufferSize, const void* data);

    GLuint handle;
    void bind();
    void unbind();

    GLuint getVertexCount() { return vertexCount; }
    GLuint getIndexCount() { return indexCount;  }

    // I believe the driver won't give us zeros for buffers, but I really don't know.
    bool isIndexed() { return indicesBuffer != 0; }
};
