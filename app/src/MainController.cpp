//
// Created by owl on 9/13/26.
//

#include <MainController.hpp>

#include "engine/platform/PlatformController.hpp"

#include <spdlog/spdlog.h>

namespace app {
    void MainController::initialize() {
        spdlog::info("Main Controller Initialized.");
    }
    bool MainController::loop() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
            return false;
        }
        return true;
    }
    }// namespace app