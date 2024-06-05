#include "mesh.h"
#include "log.h"

int Mesh::getDataSize(GLenum type) {
    switch (type) {
    case GL_FLOAT:
        return sizeof(float);
    default:
        throw std::runtime_error("Unknown data size for provided GLenum type.");
    }
}

Mesh::Mesh(MeshUsage usage) {
    if (usage == MeshUsage::Static) {
        this->usage = GL_STATIC_DRAW;
    }
    else {
        this->usage = GL_DYNAMIC_DRAW;
    }

    indicesBuffer = 0;

    glGenVertexArrays(1, &this->handle);
    this->bind();
}

Mesh::Mesh(const GLuint indices_length, const unsigned int* indices, const GLuint vertices_length, const float* vertices, const GLuint uvs_length, const float* uvs, MeshUsage usage) : Mesh(usage) {
    // vertices
    uint32_t floats_per_vertices = 3;
    if (vertices_length == uvs_length) {
        // we need a way to specify that a specific vertex info is 2d and not 3d. This is the best way I have come up with
        // to specify. (if the length of the vertices is equal to the length of the uvs, the vertices must take up 2 floats, since uvs are 2 floats
        floats_per_vertices = 2;
    }
    this->attachBuffer(GL_FLOAT, floats_per_vertices, vertices_length, (void*)vertices);
    vertexCount = vertices_length / (floats_per_vertices * sizeof(float));

    // uvs
    this->attachBuffer(GL_FLOAT, 2, uvs_length, (void*) uvs);

    // indices
    this->setIndexBuffer(indices_length, indices);

    indexCount = indices_length / sizeof(int);
}

GLuint Mesh::attachBuffer(GLenum dataType, int memberCount, GLuint dataLength, void* data) {
    this->bind();

    GLuint newIndex = (GLuint) this->vbos.size();

    GLuint newBuffer = 0;
    glGenBuffers(1, &newBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, newBuffer);
    glBufferData(GL_ARRAY_BUFFER, dataLength, data, this->usage);
    glEnableVertexAttribArray(newIndex);
    glVertexAttribPointer(newIndex, memberCount, dataType, GL_FALSE, memberCount * getDataSize(dataType), (void*) 0);
    this->vbos.push_back(newBuffer);

    return newIndex;
}

void Mesh::setIndexBuffer(const GLuint indices_length, const unsigned int* indices) {
    this->bind();

    if (indicesBuffer == 0) {
        glGenBuffers(1, &this->indicesBuffer);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indicesBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_length, indices, this->usage);
}

void Mesh::setBuffer(uint32_t bufferIdx, const GLuint bufferSize, const void* data) {
    GLuint bufferId = vbos[bufferIdx];
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, data);
}

Mesh::~Mesh() {
    
}

void Mesh::bind() {
    glBindVertexArray(this->handle);
}

void Mesh::unbind() {
    glBindVertexArray(0);
}
