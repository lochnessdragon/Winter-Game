#include "mesh.h"
#include "log.h"

Mesh::Mesh(const GLuint vertices_length, const float* vertices, const GLuint indices_length, const int* indices, const GLuint uvs_length, const float* uv_data) {
	glGenVertexArrays(1, &this->handle);
    this->bind();

    glGenBuffers(VBO_COUNT, vbos);

    // vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, vertices_length, vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	
    // uvs
    glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
    glBufferData(GL_ARRAY_BUFFER, uvs_length, uv_data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);

    // indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_length, indices, GL_STATIC_DRAW);
}

Mesh::~Mesh() {
    
}

void Mesh::bind() {
    glBindVertexArray(this->handle);
}

void Mesh::unbind() {
    glBindVertexArray(0);
}