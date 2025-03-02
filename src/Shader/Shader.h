#pragma once
#include <fwd.hpp>
#include <string>

#ifndef SHADER_H
#define SHADER_H

class Shader {
public:
    unsigned int ID;

    Shader(const char *vertexPath, const char *fragmentPath);

    ~Shader();

    void Bind() const;
    void Unbind() const;

    void setBool(const std::string &name, bool value) const;

    void setInt(const std::string &name, int value) const;

    void setFloat(const std::string &name, float value) const;

    void setFloat4(const std::string &name, float v0, float v1, float v2, float v3) const;
    void setUniform1i(const std::string &name, int value) const;
    void setUniform1f(const std::string &name, float value) const;
    void setVec3Uniform(const std::string &name, glm::vec3 value) const;
    void setUniformMatrix4fv(const std::string &name, const glm::mat4 *transform) const;
};

#endif
