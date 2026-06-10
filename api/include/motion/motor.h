#ifndef API_MOTION_MOTOR_H
#define API_MOTION_MOTOR_H

#include <SFML/System/Vector2.hpp>

namespace api::motion {
    // Tiny linear mover: each Update() steps position_ toward destination_ at a
    // fixed speed, snapping onto the destination once it is within reach.
    // Provided to students as-is for the rough NPC (no pathfinding).
    class Motor {
        sf::Vector2f position_;
        sf::Vector2f destination_;
        float speed_ = 0.f;
        float remainingDistance_ = 0.f;

    public:
        void Update(float dt); // call once per frame
        [[nodiscard]] float remaining_distance() const{ return remainingDistance_; }
        [[nodiscard]] const sf::Vector2f &position() const{ return position_; }

        void set_speed(float speed){ speed_ = speed; }
        void set_position(sf::Vector2f position);
        void set_destination(sf::Vector2f destination);

    };
} // namespace api::motion

#endif  // API_MOTION_MOTOR_H
