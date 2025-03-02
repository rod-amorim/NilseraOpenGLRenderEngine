#pragma once
#include <memory>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vector>

struct Vertex;
class Mesh;

class TMap {
public:
    explicit TMap(const char *path);
    std::vector<Vertex> m_vertex;
    std::vector<unsigned int> m_indices;
private:
    void loadTmap(const char *path);

    void parseTmap(const std::string &map);

    void parseVertices(std::string &line);
    void parseIndices();
    void parseTexCoords(std::string &line);

};
