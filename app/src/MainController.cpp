//
// Created by owl on 9/13/26.
//

#include <MainController.hpp>

#include "engine/graphics/GraphicsController.hpp"
#include "engine/graphics/OpenGL.hpp"
#include "engine/platform/PlatformController.hpp"
#include "engine/resources/ResourcesController.hpp"

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
    void MainController::draw_cube() {
        // Model
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        engine::resources::Model * cube = resources->model("barLow");




        // Shader

        engine::resources::Shader * shader = resources->shader("basic");

        shader->use();
        shader->set_mat4("projection",graphics->projection_matrix());
        shader->set_mat4("view",graphics->camera()->view_matrix());
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
        model = glm::scale(model, glm::vec3(0.5f));
        shader->set_mat4("model",model);


        cube->draw(shader);
    }
    void MainController::draw() {
        draw_cube();
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
            speed = 2.5f;
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
    void MainController::update() {
        update_camera();
    }
    }// namespace app