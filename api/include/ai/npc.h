#ifndef API_AI_NPC_H
#define API_AI_NPC_H

#include <mdspan>
#include <memory>
#include <optional>
#include <random>
#include <string_view>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include "a_star_graph.h"
#include "a_star_path.h"
#include "ai/bt_action.h"
#include "ai/bt_node.h"
#include "motion/motor.h"
#include "tiles/tile.h"

namespace api::ai {
    // A "rough" NPC: a sprite that wanders the map, driven by a behaviour tree.
    // No pathfinding -- movement is handled by a simple linear Motor.
    class Npc final {

    public:
        void Setup (const sf::Texture*, sf::Vector2i, sf::Vector2i, AStarGraph&);
        void Update(float dt);
        void Draw(sf::RenderWindow &window);
        [[nodiscard]] sf::Vector2f Position() const;

        Npc() = default;
        ~Npc() = default;
        Npc(const Npc &) = delete;
        Npc &operator=(const Npc &) = delete;

        Npc(Npc &&npc) noexcept {
            std::println("Npc : Move semantic move constructor");
            std::swap(world_size_, npc.world_size_);
            std::swap(sprite_, npc.sprite_);
            std::swap(motor_, npc.motor_);
            std::swap(bt_root_, npc.bt_root_);
            std::swap(astar_graph_, npc.astar_graph_);
        }

        Npc &operator=(Npc &&npc) noexcept{
            std::println("Npc : Move semantic move operator");
            std::swap(world_size_, npc.world_size_);
            std::swap(sprite_, npc.sprite_);
            std::swap(motor_, npc.motor_);
            std::swap(bt_root_, npc.bt_root_);
            std::swap(astar_graph_, npc.astar_graph_);
            return *this;
        }


    private:
        // Behaviour-tree actions (bound into the tree via lambdas in Setup()).
        [[nodiscard]] core::ai::behaviour_tree::Status WaitForPath();
        [[nodiscard]] core::ai::behaviour_tree::Status PickRandomDestination();
        [[nodiscard]] core::ai::behaviour_tree::Status MoveToDestination();
        [[nodiscard]] core::ai::behaviour_tree::Status Locked();

        static constexpr float kSpeed = 200.f;

        sf::Vector2i world_size_{};
        //sf::Texture* texture_=nullptr;
        std::optional<sf::Sprite> sprite_;
        motion::Motor motor_;
        std::unique_ptr<core::ai::behaviour_tree::Node> bt_root_;

        AStarGraph* astar_graph_;
        AStarPath path_;
        size_t path_idx_ = 0;

    };
} // namespace api::ai

#endif  // API_AI_NPC_H
