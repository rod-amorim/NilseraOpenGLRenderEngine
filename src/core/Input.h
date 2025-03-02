#pragma once

class Input {
public:
    Input();

    ~Input();

    void ProcessInput();

    bool ShouldQuit() const;

    static bool IsKeyPressed(int key);

private:
    bool quit;
};
