#include <iostream>

#include "core/Application.h"

int main() {
    Application app;
    app.Run();

    std::cout << "out of Run() method" << std::endl;
    return 0;
}
