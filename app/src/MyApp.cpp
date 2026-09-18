#include <MyApp.hpp>
#include <spdlog/spdlog.h>
#include <MainController.hpp>

namespace app {
    void MyApp::app_setup(){
        spdlog::info("App setup complete");
        auto mainController = register_controller<app::MainController>();
        mainController->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());

    }
}