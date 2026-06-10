#ifndef API_AI_NPC_H
#define API_AI_NPC_H

#include <memory>
#include <optional>
#include <random>
#include <string_view>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include "ai/bt_action.h"
#include "ai/bt_node.h"
#include "motion/motor.h"

namespace api::ai {
    // A "rough" NPC: a sprite that wanders the map, driven by a behaviour tree.
    // No pathfinding -- movement is handled by a simple linear Motor.
    class Npc {
    public:
        void Setup(std::string_view sprite_path, sf::Vector2f world_size,
                   sf::Vector2f start_position);

        void Update(float dt);

        void Draw(sf::RenderWindow &window);

    private:
        // Behaviour-tree actions (bound into the tree via lambdas in Setup()).
        core::ai::behaviour_tree::Status PickRandomDestination();

        [[nodiscard]] core::ai::behaviour_tree::Status MoveToDestination() const;

        core::ai::behaviour_tree::Status Locked();

        static constexpr float kSpeed = 200.f;

        std::unique_ptr<sf::Texture> texture_ = std::make_unique<sf::Texture>();
        std::optional<sf::Sprite> sprite_;

        motion::Motor motor_;
        std::unique_ptr<core::ai::behaviour_tree::Node> bt_root_;

        sf::Vector2f world_size_{};
        std::mt19937 rng_{std::random_device{}()};
    };
} // namespace api::ai

#endif  // API_AI_NPC_H
