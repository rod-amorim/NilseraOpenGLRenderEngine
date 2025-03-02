#include "VertexArray.h"

#include <iostream>

#include "VertexBuffer/VertexBuffer.h"
#include "VertexBufferLayout/VertexBufferLayout.h"

VertexArray::VertexArray() {
    glGenVertexArrays(1, &ID);
    glBindVertexArray(ID);
}

VertexArray::~VertexArray() {
    std::cout << "Destroying VertexArray" << std::endl;
    glDeleteVertexArrays(1, &ID);
}

void VertexArray::AddBuffer(VertexBuffer &vertexBuffer, VertexBufferLayout &vertexBufferLayout) {
    Bind();
    vertexBuffer.Bind();
    const auto &elements = vertexBufferLayout.GetElements();

    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); ++i) {
        const auto &element = elements[i];

        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, vertexBufferLayout.GetStride(),
                              (void *) offset);
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const {
    glBindVertexArray(ID);
}

void VertexArray::Unbind() const {
    glBindVertexArray(0);
}
