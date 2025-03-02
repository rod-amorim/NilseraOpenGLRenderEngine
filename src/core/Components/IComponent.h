#pragma once
#include <string>

class GameObject;

class IComponent {

public:
    GameObject *gameObject;

    virtual ~IComponent() = default;

    virtual void start() = 0;

    virtual void update() = 0;

    virtual std::string getName() const = 0;
private:
};
