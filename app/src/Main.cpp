#include "../include/MainController.hpp"


#include <MyApp.hpp>
#include <engine/core/Engine.hpp>
/**
 * Start here...
 */
int main(int argc, char **argv) {
    auto myApp = std::make_unique<app::MyApp>();
    return myApp->run(argc, argv);
}
