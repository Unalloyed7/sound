#include "application.h"

#include <exception>
#include <iostream>

int main(int argc, char* argv[]) {
    Application app;
    try {
        app.configure();
        return app.start(argc, argv);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return -1;
    }
    catch (...) {
        std::cerr << "Unknown error\n";
        return -2;
    }
}
