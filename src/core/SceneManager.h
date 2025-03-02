#pragma once
#include <memory>

#include "Scene.h"

class SceneManager {
public:
    SceneManager() {
    }

    static SceneManager &getInstance() {
        static SceneManager instance;
        return instance;
    }

    SceneManager(const SceneManager &) = delete;

    SceneManager &operator=(const SceneManager &) = delete;

    //attributes
    std::shared_ptr<Scene> current;

private:
};
