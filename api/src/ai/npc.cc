#include "ai/npc.h"

#include <format>
#include <print>
#include <SFML/Graphics/RectangleShape.hpp>

#include "ai/bt_action.h"
#include "ai/bt_node_factory.h"
#include "ai/bt_selector.h"
#include "ai/bt_sequence.h"
#include "ai/a_star_graph.h"
#include "rng/rng.h"


namespace api::ai {
    using core::ai::behaviour_tree::Status;

    void Npc::Setup(const sf::Texture *texture, sf::Vector2i world_size, sf::Vector2i start_position,
                    AStarGraph &astar_graph){
        world_size_ = world_size;

        if (texture != nullptr) {
            sprite_ = sf::Sprite(*texture);
          // ← REMPLACE sprite_->setOrigin({16,16})
          const sf::Vector2u textureSize = texture->getSize();
          sprite_->setScale({
              32.f / static_cast<float>(textureSize.x),
              32.f / static_cast<float>(textureSize.y)
          });
          // Centre l'origine sur le sprite déjà scalé
          sprite_->setOrigin({
              static_cast<float>(textureSize.x) / 2.f,
              static_cast<float>(textureSize.y) / 2.f
          });
        }

        sf::Vector2f start_position_f = sf::Vector2f{static_cast<float>(start_position.x), static_cast<float>(start_position.y)};

        motor_.set_position(start_position_f);
        motor_.set_destination(start_position_f); // stay put until the first pick
        motor_.set_speed(kSpeed);

        using namespace core::ai::behaviour_tree::node_factory;

        auto testSequence = MakeSequence();
        testSequence->AddChild(MakeAction([] { return Status::kSuccess; }));
        testSequence->AddChild(MakeAction([] { return Status::kFailure; }));
        testSequence->AddChild(MakeAction([this] { return Locked(); }));

        auto wanderSequence = MakeSequence();
        wanderSequence->AddChild(MakeAction([this] { return WaitForPath(); }));
        wanderSequence->AddChild(MakeAction([this] { return PickRandomDestination(); }));
        wanderSequence->AddChild(MakeAction([this] { return MoveToDestination(); }));

        auto basicSelector = MakeSelector();
        basicSelector->AddChild(std::move(testSequence));
        basicSelector->AddChild(std::move(wanderSequence));

        bt_root_ = std::move(basicSelector);

        // Rough wander behavior :
        // Sequence (pick a random destination, then move to it)
        // PickRandomDestination always succeeds, MoveToDestination stays kRunning
        // until the motor reaches the target. When the sequence completes, it resets
        astar_graph_ = &astar_graph;
    }


    void Npc::Update(const float dt){
        motor_.Update(dt);
        if (bt_root_) {
            bt_root_->Tick();
        }
    }

    void Npc::Draw(sf::RenderWindow &window){
        if (sprite_.has_value()) {

            // sf::RectangleShape rect{sprite_->getLocalBounds().size};
            // sprite_->setPosition({32,32});
            // rect.setPosition(sprite_->getPosition());
            // window.draw(rect);

            sprite_->setPosition(motor_.position());
            window.draw(*sprite_);

        }
    }

    Status Npc::WaitForPath(){
        return Status::kSuccess;
    }

    Status Npc::PickRandomDestination(){
        // core::rng::get_value<long long>(0, walkable_tiles_.extent(0) * walkable_tiles_.extent(1));
        // get the path
        sf::Vector2i destination = astar_graph_->GetRandomNode();
        if (ManhattanDistance(sf::Vector2i{motor_.position()}, destination) > 200) {
            return Status::kFailure;
        }
        path_ = astar_graph_->GetPath(sf::Vector2i{motor_.position()}, destination);

        if (!path_.empty()) {
            path_idx_ = 0;
            motor_.set_destination(sf::Vector2f{path_[path_idx_]});
            return Status::kSuccess;
        }

        return Status::kFailure;
    }

    Status Npc::MoveToDestination(){
        // on parcourt case par case, waypoints
        // TODO : path increment
        if (motor_.remaining_distance() <= 0.001f) {
            path_idx_++;
            if (path_idx_ >= path_.size()) {
                return Status::kSuccess;
            } else {
                motor_.set_destination(sf::Vector2f{path_[path_idx_]});
            }
        }

        return Status::kRunning;
    }

    Status Npc::Locked(){
        std::println("locked");
        return Status::kRunning;
    }
} // namespace api::ai
