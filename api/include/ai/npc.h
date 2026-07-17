#ifndef API_AI_NPC_H
#define API_AI_NPC_H

#include <mdspan>
#include <memory>
#include <optional>
#include <functional>
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
#include "resources/resource_type.h"

namespace api::ai {
    class Npc final {

    public:
        using FindResourceFn   = std::function<std::optional<sf::Vector2f>(sf::Vector2f, ResourceTile)>;
        using RemoveResourceFn = std::function<void(sf::Vector2f)>;
        using UnreserveFn      = std::function<void(sf::Vector2f)>;

        void Setup(const sf::Texture*, sf::Vector2i, sf::Vector2i, AStarGraph&,
                   sf::Vector2f house_pos, ResourceTile resource_type,
                   FindResourceFn find_fn, RemoveResourceFn remove_fn, UnreserveFn unreserve_fn);

        void Update(float dt);
        void Draw(sf::RenderWindow& window);
        [[nodiscard]] sf::Vector2f Position() const;

        Npc() = default;
        ~Npc() = default;
        Npc(const Npc&) = delete;
        Npc& operator=(const Npc&) = delete;

        // ← 2. MOVE SEMANTICS — swapper les nouveaux membres aussi
        Npc(Npc&& npc) noexcept {
            std::swap(world_size_, npc.world_size_);
            std::swap(sprite_, npc.sprite_);
            std::swap(motor_, npc.motor_);
            std::swap(bt_root_, npc.bt_root_);
            std::swap(astar_graph_, npc.astar_graph_);
            std::swap(house_pos_, npc.house_pos_);
            std::swap(resource_type_, npc.resource_type_);
            std::swap(find_resource_, npc.find_resource_);
            std::swap(remove_resource_, npc.remove_resource_);
          std::swap(unreserve_resource_, npc.unreserve_resource_);
          std::swap(dt_, npc.dt_);
          std::swap(path_, npc.path_);
        }

        Npc& operator=(Npc&& npc) noexcept {
            std::swap(world_size_, npc.world_size_);
            std::swap(sprite_, npc.sprite_);
            std::swap(motor_, npc.motor_);
            std::swap(bt_root_, npc.bt_root_);
            std::swap(astar_graph_, npc.astar_graph_);
            std::swap(house_pos_, npc.house_pos_);
            std::swap(resource_type_, npc.resource_type_);
            std::swap(find_resource_, npc.find_resource_);
            std::swap(remove_resource_, npc.remove_resource_);
            std::swap(unreserve_resource_, npc.unreserve_resource_);
            std::swap(dt_, npc.dt_);
            std::swap(path_, npc.path_);
            return *this;
        }

    private:
        sf::Vector2f     house_pos_{};
        ResourceTile     resource_type_ = ResourceTile::kWood;
        sf::Vector2f     current_resource_pos_{};
        float            harvest_timer_   = 0.f;
        float            home_wait_timer_ = 0.f;
        float            search_cooldown_ = 0.f;
        float             dt_ = 0.f;

        FindResourceFn   find_resource_;
        RemoveResourceFn remove_resource_;
        UnreserveFn unreserve_resource_;

        [[nodiscard]] core::ai::behaviour_tree::Status FindResource();
        [[nodiscard]] core::ai::behaviour_tree::Status MoveToResource();
        [[nodiscard]] core::ai::behaviour_tree::Status Harvest();
        [[nodiscard]] core::ai::behaviour_tree::Status ReturnHome();
        [[nodiscard]] core::ai::behaviour_tree::Status WaitAtHome();

        /*[[nodiscard]] core::ai::behaviour_tree::Status WaitForPath();
        [[nodiscard]] core::ai::behaviour_tree::Status PickRandomDestination();
        [[nodiscard]] core::ai::behaviour_tree::Status MoveToDestination();
        [[nodiscard]] core::ai::behaviour_tree::Status Locked();*/

        static constexpr float kSpeed = 200.f;

        sf::Vector2i world_size_{};
        std::optional<sf::Sprite> sprite_;
        motion::Motor motor_;
        std::unique_ptr<core::ai::behaviour_tree::Node> bt_root_;

        AStarGraph* astar_graph_;
        AStarPath   path_;
        // size_t path_idx_ = 0;  ← SUPPRIMÉ
    };

} // namespace api::ai

#endif  // API_AI_NPC_H
