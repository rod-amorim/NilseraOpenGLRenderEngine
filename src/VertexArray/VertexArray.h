#pragma once

class VertexBuffer;
class VertexBufferLayout;

class VertexArray {
private:
    unsigned int ID;

public:
    VertexArray();

    ~VertexArray();

    void AddBuffer(VertexBuffer &vertexBuffer, VertexBufferLayout &vertexBufferLayout);

    void Bind() const;
    void Unbind() const;
};
