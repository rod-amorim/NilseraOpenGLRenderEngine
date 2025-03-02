#include "TMap.h"

#include <fstream>
#include <geometric.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>

#include "core/Structs/Vertex.h"

struct Vec3Comparator {
    bool operator()(const glm::vec3 &a, const glm::vec3 &b) const {
        if (a.x != b.x)
            return a.x < b.x;
        if (a.y != b.y)
            return a.y < b.y;
        return a.z < b.z;
    }
};

TMap::TMap(const char *path) {
    loadTmap(path);
}

glm::vec3 calculateNormal(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3) {
    glm::vec3 vector1 = v2 - v1;
    glm::vec3 vector2 = v3 - v1;
    glm::vec3 normal = glm::cross(vector2, vector1);
    return normalize(normal);
}

void TMap::loadTmap(const char *path) {
    std::string mapString;
    std::ifstream mapFile;

    mapFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open file
        mapFile.open(path);

        // read file's buffer contents into streams
        std::stringstream mapStream;
        mapStream << mapFile.rdbuf();

        // close file handlers
        mapFile.close();

        // convert stream into string
        mapString = mapStream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::.MAP::FILE_NOT_SUCCESFULLY_PARSED" << std::endl;
    }

    // std::cout << mapString << std::endl;

    parseTmap(mapString);
}

void TMap::parseTmap(const std::string &map) {
    std::istringstream contentStream(map);
    std::string line;

    while (std::getline(contentStream, line)) {
        if (line.find('(') != std::string::npos) {
            parseVertices(line);
            parseTexCoords(line);
        }
    }
    parseIndices();
}

void TMap::parseVertices(std::string &line) {
    std::vector<Vertex> vertexList;

    size_t start = 0;
    std::vector<glm::vec3> thisFaceVertices;
    while ((start = line.find('(', start)) != std::string::npos) {
        size_t end = line.find(')', start);

        if (end == std::string::npos)
            break;

        std::string vertexData = line.substr(start + 1, end - start - 1);
        std::istringstream stream(vertexData);
        float x, y, z;

        stream >> x >> y >> z;
        // Corrigindo a rotação de 90 graus
        float correctedX = x;
        float correctedY = z; // Trocar Y e Z
        float correctedZ = -y; // Trocar Y e Z e inverter o eixo Z

        thisFaceVertices.push_back(glm::vec3(correctedX / 16, correctedY / 16, correctedZ / 16));
        start = end + 1;
    }

    //calculate the v4 missing vertices
    const glm::vec3 &vertex1 = thisFaceVertices[0];
    const glm::vec3 &vertex2 = thisFaceVertices[1];
    const glm::vec3 &vertex3 = thisFaceVertices[2];

    const glm::vec3 vertex4 = vertex1 + vertex3 - vertex2;

    //UV
    start = 0;
    std::vector<glm::vec4> uvInfoList;
    while ((start = line.find('[', start)) != std::string::npos) {
        size_t end = line.find(']', start);

        if (end == std::string::npos)
            break;

        std::string vertexData = line.substr(start + 1, end - start - 1);
        std::istringstream stream(vertexData);
        float x, y, z, w;

        stream >> x >> y >> z >> w;

        uvInfoList.push_back(glm::vec4(x, y, z, w));
        start = end + 1;
    }

    glm::vec4 u_uv = uvInfoList[0];
    glm::vec4 v_uv = uvInfoList[1];

    glm::vec3 normal = calculateNormal(vertex1, vertex2, vertex3);

    glm::vec3 u_uv_direction = glm::vec3(u_uv.x, u_uv.y, u_uv.z);
    glm::vec3 v_uv_direction = glm::vec3(v_uv.x, v_uv.y, v_uv.z);

    float u1 = vertex1.x * u_uv_direction.x + vertex1.y * u_uv_direction.y + vertex1.z * u_uv_direction.z;
    float v1 = vertex1.x * v_uv_direction.x + vertex1.y * v_uv_direction.y + vertex1.z * v_uv_direction.z;

    float u2 = vertex2.x * u_uv_direction.x + vertex2.y * u_uv_direction.y + vertex2.z * u_uv_direction.z;
    float v2 = vertex2.x * v_uv_direction.x + vertex2.y * v_uv_direction.y + vertex2.z * v_uv_direction.z;

    float u3 = vertex3.x * u_uv_direction.x + vertex3.y * u_uv_direction.y + vertex3.z * u_uv_direction.z;
    float v3 = vertex3.x * v_uv_direction.x + vertex3.y * v_uv_direction.y + vertex3.z * v_uv_direction.z;

    float u4 = vertex4.x * u_uv_direction.x + vertex4.y * u_uv_direction.y + vertex4.z * u_uv_direction.z;
    float v4 = vertex4.x * v_uv_direction.x + vertex4.y * v_uv_direction.y + vertex4.z * v_uv_direction.z;

    //dot (vertexPos,face normal) * scale + offset
    // float u1 = dot(tangent, vertex1) + u_uv.w;
    // float v1 = dot(bitangent, vertex1) + v_uv.w;
    //
    // float u2 = dot(tangent, vertex2) + u_uv.w;
    // float v2 = dot(bitangent, vertex2) + v_uv.w;
    //
    // float u3 = dot(tangent, vertex3) + u_uv.w;
    // float v3 = dot(bitangent, vertex3) + v_uv.w;
    //
    // float u4 = dot(tangent, vertex4) + u_uv.w;
    // float v4 = dot(bitangent, vertex4) + v_uv.w;

    m_vertex.push_back({vertex1, normal, glm::vec2(u1, v1)});
    m_vertex.push_back({vertex2, normal, glm::vec2(u2, v2)});
    m_vertex.push_back({vertex3, normal, glm::vec2(u3, v3)});
    m_vertex.push_back({vertex4, normal, glm::vec2(u4, v4)});
}

void TMap::parseIndices() {
    for (size_t i = 0; i < m_vertex.size(); i += 4) {
        m_indices.push_back(i); // v1
        m_indices.push_back(i + 2); // v3
        m_indices.push_back(i + 1); // v2

        m_indices.push_back(i); // v1
        m_indices.push_back(i + 3); // v4
        m_indices.push_back(i + 2); // v3
    }
}

void TMap::parseTexCoords(std::string &line) {
}
