//
// Created by sebas on 04.06.2026.
//

#include "motion/motor.h"

namespace api::motion {
    void Motor::Update(const float dt) {
        const sf::Vector2f distance = destination_ - position_;
        remainingDistance_ = distance.length();

        if (remainingDistance_ < speed_ * dt) {
            position_ = destination_;
            remainingDistance_ = 0.f;
            return;
        }
        position_ += distance.normalized() * speed_ * dt;
    }

    void Motor::set_position(sf::Vector2i position){
        position_ = sf::Vector2f(position);
        remainingDistance_ = (destination_ - position_).length();
    }

    void Motor::set_destination(sf::Vector2i destination){
        destination_ = sf::Vector2f(destination);
        remainingDistance_ = (destination_ - position_).length();
    }
}