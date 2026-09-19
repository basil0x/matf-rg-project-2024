//
// Created by owl on 9/13/26.
//

#include <MainController.hpp>

#include "engine/graphics/GraphicsController.hpp"
#include "engine/graphics/OpenGL.hpp"
#include "engine/platform/PlatformController.hpp"
#include "engine/resources/ResourcesController.hpp"
#include "imgui.h"

#include <spdlog/spdlog.h>

namespace app {

class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
public:
    void on_mouse_move(engine::platform::MousePosition position) override;
};
void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
    auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
    camera->rotate_camera(position.dx, position.dy);
}

void MainController::initialize() {
    spdlog::info("Main Controller Initialized.");
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
    engine::graphics::OpenGL::enable_depth_testing();
}
bool MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
        return false;
    }
    return true;
}
void MainController::draw_bar() {
    // Model
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    engine::resources::Model *bar = resources->model("room");


    // Shader

    engine::resources::Shader *shader = resources->shader("lighting");

    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    shader->set_vec3("viewPos", graphics->camera()->Position);

    shader->set_vec3("dirLightDirection", m_dir_light_direction);
    shader->set_vec3("dirLightColor", m_dir_light_color);
    shader->set_float("dirLightIntensity", m_dir_light_intensity);

    shader->set_vec3("pointLightPos", m_point_light_pos);
    shader->set_vec3("pointLightColor", m_point_light_color);
    shader->set_float("pointLightIntensity", m_point_light_intensity);


    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f));
    shader->set_mat4("model", model);


    bar->draw(shader);
}
void MainController::draw_gui() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    graphics->begin_gui();
    ImGui::Begin("Lighting");
    ImGui::SeparatorText("Directional Light");
    ImGui::DragFloat3("Direction", &m_dir_light_direction.x, 0.01f);
    ImGui::ColorEdit3("Dir Color", &m_dir_light_color.x);
    ImGui::SliderFloat("Dir Intensity", &m_dir_light_intensity, 0.0f, 3.0f);
    ImGui::SeparatorText("Point Light");
    ImGui::DragFloat3("Point Pos", &m_point_light_pos.x, 0.1f);
    ImGui::ColorEdit3("Point Color", &m_point_light_color.x);
    ImGui::SliderFloat("Point Intensity", &m_point_light_intensity, 0.0f, 3.0f);
    ImGui::End();

    ImGui::Begin("Controls");
    ImGui::Text("Camera     - W A S D + Mouse");
    ImGui::Text("GUI toggle - TAB");
    ImGui::Text("Event Demo - 1 , 2 , 3 etc...");
    ImGui::End();
    graphics->end_gui();
}
void MainController::draw_skybox() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    graphics->draw_skybox(resources->shader("skybox"), resources->skybox("skybox"));
}
void MainController::draw() {
    draw_bar();
    draw_skybox();

    // TODO: Fix Camera Reposition on toggle
    if (m_interact_mode) {
        draw_gui();
    }
}
void MainController::begin_draw() {
    engine::graphics::OpenGL::clear_buffers();
}
void MainController::end_draw() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->swap_buffers();
}
void MainController::update_camera() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto camera = graphics->camera();

    float dt = platform->dt();

    float speed = 1.0f;

    if (platform->key(engine::platform::KeyId::KEY_LEFT_SHIFT).is_down()) {
        speed = 3.5f;
    }

    if (platform->key(engine::platform::KeyId::KEY_W).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::FORWARD, dt * speed);
    }
    if (platform->key(engine::platform::KeyId::KEY_S).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt * speed);
    }
    if (platform->key(engine::platform::KeyId::KEY_A).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::LEFT, dt * speed);
    }
    if (platform->key(engine::platform::KeyId::KEY_D).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::RIGHT, dt * speed);
    }
}
void MainController::update_events() {
    float dt = engine::core::Controller::get<engine::platform::PlatformController>()->dt();
    m_timed_events.update(dt);
}
void MainController::update() {
    update_camera();
    update_events();
}
void MainController::poll_events() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();

    // Gui Interaction
    if (platform->key(engine::platform::KEY_TAB).state() == engine::platform::Key::State::JustPressed) {
        m_interact_mode = !m_interact_mode;
        platform->set_enable_cursor(m_interact_mode);
    }

    // Events
    if (platform->key(engine::platform::KEY_1).state() == engine::platform::Key::State::JustPressed) {
        m_timed_events.add(2.0f, [this]() {
            m_point_light_intensity = 0.0f;
        });

        m_timed_events.add(6.0f, [this]() {
            m_point_light_color = glm::vec3(1.0f, 0.0f, 1.0f);
            m_point_light_intensity = 1.5f;
        });

        m_timed_events.add(5.0f, [this]() {
            m_dir_light_intensity = 0.0f;
        });
    }
}
}// namespace app