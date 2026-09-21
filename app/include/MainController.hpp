//
// Created by owl on 9/13/26.
//

#ifndef MATF_RG_PROJECT_2024_MAINCONTROLLER_HPP
#define MATF_RG_PROJECT_2024_MAINCONTROLLER_HPP

#include "glm/vec3.hpp"
#include <engine/util/TimedEvent.hpp>


#include <engine/core/Controller.hpp>

#include <functional>

namespace app {


class MainController : public engine::core::Controller {
    void initialize() override;

    bool loop() override;
    void draw_bar();
    void draw_gui();
    void draw_skybox();
    void draw() override;
    void begin_draw() override;
    void end_draw() override;
    void update_camera();
    void update_events();
    void update() override;
    void poll_events() override;

    //Events

    engine::util::TimedEventManager m_timed_events;


    //Modes
    bool m_interact_mode{true};


    //Lighting

    glm::vec3 m_dir_light_direction{-0.2f, -1.0f, -0.3f};
    glm::vec3 m_dir_light_color{1.0f, 1.0f, 1.0f};
    float m_dir_light_intensity{1.0f};

    glm::vec3 m_point_light_pos{0.0f, 5.0f, 0.0f};
    glm::vec3 m_point_light_color{1.0f, 0.8f, 0.4f};
    float m_point_light_intensity{1.0f};

public:
    std::string_view name() const override {
        return "app:MainController";
    }
};
}// namespace app

#endif//MATF_RG_PROJECT_2024_MAINCONTROLLER_HPP
