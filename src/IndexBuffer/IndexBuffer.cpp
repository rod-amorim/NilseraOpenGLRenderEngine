#include "IndexBuffer.h"

#include <iostream>
#include <GL/glew.h>

#include "core/Mesh/Mesh.h"

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int size)
    : m_Count(size) {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
    std::cout << "Destroying Index Buffer" << std::endl;

    glDeleteBuffers(1, &ID);
}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void IndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
