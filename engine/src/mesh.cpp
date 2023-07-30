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

Mesh::Mesh(const GLuint indices_length, const unsigned int* indices, const GLuint vertices_length, const float* vertices, const GLuint uvs_length, const float* uvs) {
	glGenVertexArrays(1, &this->handle);
    this->bind();

    // vertices
    this->attachBuffer(GL_FLOAT, 3, vertices_length, (void*) vertices);
    vertexCount = vertices_length / (3 * sizeof(float));
    //Log::getRendererLog()->trace("Vertex Count: {} Array Size: {}", vertexCount, vertices_length);

    // uvs
    this->attachBuffer(GL_FLOAT, 2, uvs_length, (void*) uvs);
    
    // normals
    // this->attachBuffer(GL_FLOAT, 3, normals_length, (void*) normals);

    //// vertices
    //glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    //glBufferData(GL_ARRAY_BUFFER, vertices_length, vertices, GL_STATIC_DRAW);
    //glEnableVertexAttribArray(0);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	
    //// uvs
    //glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
    //glBufferData(GL_ARRAY_BUFFER, uvs_length, uv_data, GL_STATIC_DRAW);
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);

    // indices
    glGenBuffers(1, &this->indicesBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indicesBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_length, indices, GL_STATIC_DRAW);

    indexCount = indices_length / sizeof(int);
}

GLuint Mesh::attachBuffer(GLenum dataType, int memberCount, GLuint dataLength, void* data) {
    this->bind();

    GLuint newIndex = (GLuint) this->vbos.size();

    GLuint newBuffer = 0;
    glGenBuffers(1, &newBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, newBuffer);
    glBufferData(GL_ARRAY_BUFFER, dataLength, data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(newIndex);
    glVertexAttribPointer(newIndex, memberCount, dataType, GL_FALSE, memberCount * getDataSize(dataType), (void*) 0);
    this->vbos.push_back(newBuffer);

    return newIndex;
}

Mesh::~Mesh() {
    
}

void Mesh::bind() {
    glBindVertexArray(this->handle);
}

void Mesh::unbind() {
    glBindVertexArray(0);
}
