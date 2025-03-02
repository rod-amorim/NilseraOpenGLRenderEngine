#include "VertexBuffer.h"

#include <iostream>
#include <GL/glew.h>

#include "core/Mesh/Mesh.h"

class Mesh;

VertexBuffer::VertexBuffer(const void *data, unsigned int size) {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
    std::cout << "Destroying Vertex Buffer" << std::endl;
    glDeleteBuffers(1, &ID);
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
