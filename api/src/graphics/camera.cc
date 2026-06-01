#include "graphics/camera.h"

#include <algorithm>
#include <cmath>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

namespace graphics {

    void Camera::Setup(sf::Vector2f windowSize) {
        view_.setSize(windowSize);
        view_.setCenter(windowSize * 0.5f);
        currentZoom_ = 1.f;
        lastWindowSize_ = windowSize;
    }

    void Camera::Apply(sf::RenderWindow &window) {
        const sf::Vector2u currentSize = window.getSize();
        if (static_cast<float>(currentSize.x) != lastWindowSize_.x ||
            static_cast<float>(currentSize.y) != lastWindowSize_.y) {
            OnWindowResized(currentSize);
        }
        window.setView(view_);
    }

    void Camera::Zoom(float factor, sf::Vector2f anchorWorld) {
        const float target = std::clamp(currentZoom_ * factor, minZoom_, maxZoom_);
        const float applied = target / currentZoom_;
        if (applied == 1.f) return;

        const sf::Vector2f oldCenter = view_.getCenter();
        view_.zoom(applied);
        currentZoom_ = target;
        view_.setCenter(anchorWorld + (oldCenter - anchorWorld) * applied);
    }

    void Camera::HandleEvent(const sf::Event &event, const sf::RenderWindow &window) {
        if (const auto *wheel = event.getIf<sf::Event::MouseWheelScrolled>()) {
            const sf::Vector2f anchor =
                window.mapPixelToCoords({wheel->position.x, wheel->position.y}, view_);
            const float factor = wheel->delta > 0 ? wheelZoomStep_ : 1.f / wheelZoomStep_;
            Zoom(factor, anchor);
            return;
        }

        if (const auto *pressed = event.getIf<sf::Event::MouseButtonPressed>()) {
            if (pressed->button == sf::Mouse::Button::Middle) {
                dragging_ = true;
                lastMousePixel_ = pressed->position;
            }
            return;
        }

        if (const auto *released = event.getIf<sf::Event::MouseButtonReleased>()) {
            if (released->button == sf::Mouse::Button::Middle) {
                dragging_ = false;
            }
            return;
        }

        if (const auto *moved = event.getIf<sf::Event::MouseMoved>()) {
            if (dragging_) {
                const sf::Vector2f prev = window.mapPixelToCoords(lastMousePixel_, view_);
                const sf::Vector2f curr = window.mapPixelToCoords(moved->position, view_);
                view_.move(prev - curr);
                lastMousePixel_ = moved->position;
            }
            return;
        }

        if (const auto *resized = event.getIf<sf::Event::Resized>()) {
            OnWindowResized(resized->size);
        }
    }

    void Camera::OnWindowResized(sf::Vector2u newSize) {
        if (newSize.x == 0 || newSize.y == 0) return;
        const sf::Vector2f sizeF{static_cast<float>(newSize.x),
                                 static_cast<float>(newSize.y)};
        if (lastWindowSize_.x <= 0.f || lastWindowSize_.y <= 0.f) {
            view_.setSize(sizeF);
        } else {
            const sf::Vector2f viewSize = view_.getSize();
            view_.setSize({viewSize.x * (sizeF.x / lastWindowSize_.x),
                           viewSize.y * (sizeF.y / lastWindowSize_.y)});
            view_.setViewport(sf::FloatRect({0.f, 0.f}, {1.f, 1.f}));
        }
        lastWindowSize_ = sizeF;
    }

    void Camera::Update(float dt) {
        using Key = sf::Keyboard::Key;
        const bool shift = sf::Keyboard::isKeyPressed(Key::LShift) ||
                           sf::Keyboard::isKeyPressed(Key::RShift);
        const bool up    = sf::Keyboard::isKeyPressed(Key::Up);
        const bool down  = sf::Keyboard::isKeyPressed(Key::Down);
        const bool left  = sf::Keyboard::isKeyPressed(Key::Left);
        const bool right = sf::Keyboard::isKeyPressed(Key::Right);

        if (shift && (up || down)) {
            // Zoom anchored on the current view center (no cursor info in keyboard path).
            const float exponent = (up ? -1.f : 1.f) * keyZoomRate_ * dt;
            const float factor = std::pow(wheelZoomStep_, exponent);
            Zoom(factor, view_.getCenter());
            return;
        }

        sf::Vector2f delta{};
        if (left)  delta.x -= 1.f;
        if (right) delta.x += 1.f;
        if (up)    delta.y -= 1.f;
        if (down)  delta.y += 1.f;

        if (delta.x != 0.f || delta.y != 0.f) {
            const float len = std::sqrt(delta.x * delta.x + delta.y * delta.y);
            delta /= len;
            view_.move(delta * panSpeed_ * dt * currentZoom_);
        }
    }

}
