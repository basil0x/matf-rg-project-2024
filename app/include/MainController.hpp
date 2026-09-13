//
// Created by owl on 9/13/26.
//

#ifndef MATF_RG_PROJECT_2024_MAINCONTROLLER_HPP
#define MATF_RG_PROJECT_2024_MAINCONTROLLER_HPP

#include <engine/core/Controller.hpp>

namespace app {


class MainController: public engine::core::Controller {
    void initialize() override;

    bool loop() override;
    void draw_cube();
    void draw() override;
    void begin_draw() override;
    void end_draw() override;
    void update_camera();
    void update() override;

public:
    std::string_view name() const override {
        return "app:MainController";
    }
};
}// namespace app

#endif//MATF_RG_PROJECT_2024_MAINCONTROLLER_HPP
