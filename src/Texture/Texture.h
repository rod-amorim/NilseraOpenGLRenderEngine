#pragma once

#include <string>
#include <assimp/types.h>

class Texture {
private:
    unsigned int ID;
    int width;
    int height;
    int nrChannels;

    unsigned char *data;
    // void Bind(unsigned int slot);

public:
    Texture(const std::string &filePath);

    Texture(std::string path, const std::string &dir);

    [[nodiscard]] unsigned int GetID() const {
        return ID;
    }

    void Bind() const;
    void Unbind();

    int textureSlot;
    std::string type;
    aiString path;
};

