#include "mesh.h"
#include "log.h"

Mesh::Mesh(const GLuint vertices_length, const float* vertices, const GLuint indices_length, const int* indices) {
    Log::getRendererLog()->trace("Creating a mesh");
	glGenVertexArrays(1, &this->handle);
    this->bind();

	Log::getRendererLog()->trace("Placing data in the buffer");
    glGenBuffers(2, vbos);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, vertices_length, vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);

	Log::getRendererLog()->trace("Applying indices buffer");
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