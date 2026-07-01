//
// Created by noahs on 25.06.2026.
//
#include "ui/clickable.h"

#include <print>

namespace api::ui {
    bool Clickable::DoHoverEnterEvents(const std::optional<sf::Event> &evt){
        // First Do Hover Events
        if (const auto mouseMove = evt->getIf<sf::Event::MouseMoved>()) {
            if (zone_.contains(mouseMove->position) && !isHover_) {
                OnHoverEnter();
                isHover_ = true;
                //std::println("Hover Enter this !");
                return true;
            }
        }
        return false;
    }

    bool Clickable::DoHoverExitEvents(const std::optional<sf::Event> &evt){
        if (const auto mouseMove = evt->getIf<sf::Event::MouseMoved>()) {
            if (!zone_.contains(mouseMove->position) && isHover_) {
                OnHoverExit();
                isHover_ = false;
                //std::println("Hover Exit this !");
                return true;
            }
        }
        return false;
    }

    bool Clickable::DoReleasedEvents(const std::optional<sf::Event> &evt){
        if (const auto released = evt->getIf<sf::Event::MouseButtonReleased>()) {
            if (zone_.contains(released->position)) {
                if (released->button == sf::Mouse::Button::Left) {
                    OnReleasedLeft();
                    return true; // Indicate that a click was handled
                }

                if (released->button == sf::Mouse::Button::Right) {
                    OnReleasedRight();
                    return true; // Indicate that a click was handled
                }
            }
        }
        return false;
    }

    bool Clickable::DoPressedEvents(const std::optional<sf::Event> &evt){
        if (const auto pressed = evt->getIf<sf::Event::MouseButtonPressed>()) {
            if (zone_.contains(pressed->position)) {
                if (pressed->button == sf::Mouse::Button::Left) {
                    OnPressedLeft();
                    return true; // Indicate that a click was handled
                }

                if (pressed->button == sf::Mouse::Button::Right) {
                    OnPressedRight();
                    return true; // Indicate that a click was handled
                }
            }
        }
        return false;
    }

    void Clickable::LeaveHover(){
        if (isHover_) {
            OnHoverExit();
            isHover_ = false;
        }
    }

    bool Clickable::HandleEvent(const std::optional<sf::Event> &event){
        if (DoReleasedEvents(event)) return true;
        if (DoPressedEvents(event)) return true;

        return false;

    }

    void Clickable::SetZone(sf::IntRect zone){
        zone_ = zone;
    }

} // namespace api::ui
