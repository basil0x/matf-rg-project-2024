/**
* @file TimedEvent.hpp
 * @brief Defines  TimedEvent struct, TimedEventManager class along with its basic functionality.
*/


#ifndef MATF_RG_PROJECT_TIMEDEVENT_HPP
#define MATF_RG_PROJECT_TIMEDEVENT_HPP

#include <functional>
#include <vector>

namespace engine::util {

struct TimedEvent {
    float delay;
    float elapsed{0.0f};
    bool triggered{false};
    std::function<void()> action;
};
/**
* @class TimedEventManager
* @brief Class for managing timed events.
*
* Supports adding functions to timed event triggers.
*/
class TimedEventManager {
public:
    /**
    * @brief Adds a new event to m_events
    */
    void add(float delay, std::function<void()> action) {
        m_events.push_back({delay, 0.0f, false, std::move(action)});
    }

    /**
    * @brief Used for resolving states of events on update in m_events
    */
    void update(float dt) {
        for (auto &event: m_events) {
            if (event.triggered) continue;
            event.elapsed += dt;
            if (event.elapsed >= event.delay) {
                event.action();
                event.triggered = true;
            }
        }
    }
    /**
    * @brief Clears m_events
    */
    void clear() {
        m_events.clear();
    }

private:
    /**
    * @brief Vector array of timed events
    */
    std::vector<TimedEvent> m_events;
};

}// namespace engine::util

#endif//MATF_RG_PROJECT_TIMEDEVENT_HPP
