#include "core/Scene.h"

class TestScene : public Scene, public std::enable_shared_from_this<Scene> {
public:
    void Setup() override;

    ~TestScene();
};
