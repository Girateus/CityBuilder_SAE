#include "ai/npc.h"

#include <format>
#include <print>

#include "ai/bt_action.h"
#include "ai/bt_node_factory.h"
#include "ai/bt_selector.h"
#include "ai/bt_sequence.h"


namespace api::ai {
    using core::ai::behaviour_tree::Status;

    void Npc::Setup(std::string_view sprite_path, const sf::Vector2f world_size,
                    const sf::Vector2f start_position){
        world_size_ = world_size;

        if (texture_->loadFromFile(std::string(sprite_path))) {
            sprite_ = sf::Sprite(*texture_);
          sf::Vector2u textureSize = texture_->getSize(); // 240x240
          sprite_->setScale({
              64.f / static_cast<float>(textureSize.x),
              64.f / static_cast<float>(textureSize.y)
          });

        }

        motor_.set_position(start_position);
        motor_.set_destination(start_position); // stay put until the first pick
        motor_.set_speed(kSpeed);

        using namespace core::ai::behaviour_tree::node_factory;

        auto testSequence = MakeSequence();
        testSequence->AddChild(MakeAction([] {return Status::kSuccess;}));
        testSequence->AddChild(MakeAction([] {return Status::kFailure;}));
        testSequence->AddChild(MakeAction([this] { return Locked();}));

        auto wanderSequence = MakeSequence();
        wanderSequence->AddChild(MakeAction([this] { return MoveToDestination(); }));
        wanderSequence->AddChild(MakeAction([this] { return PickRandomDestination(); }));

        auto basicSelector = MakeSelector();
        basicSelector->AddChild(std::move(testSequence));
        basicSelector->AddChild(std::move(wanderSequence));

        bt_root_ = std::move(basicSelector);

        // Rough wander behaviour:
        //   Sequence( pick a random destination, then move to it )
        // PickRandomDestination always succeeds, MoveToDestination stays kRunning
        // until the motor reaches the target. When the sequence completes it resets
    }

    void Npc::Update(const float dt){
        motor_.Update(dt);
        if (bt_root_) {
            bt_root_->Tick();
        }
    }

    void Npc::Draw(sf::RenderWindow &window){
        if (sprite_.has_value()) {
            sprite_->setPosition(motor_.position());
            window.draw(*sprite_);
        }
    }

    Status Npc::PickRandomDestination(){
        std::uniform_real_distribution<float> x_dist(0.f, world_size_.x);
        std::uniform_real_distribution<float> y_dist(0.f, world_size_.y);
        motor_.set_destination({x_dist(rng_), y_dist(rng_)});
        //get the path
        return Status::kSuccess;
    }

    Status Npc::MoveToDestination() const{

      //on parcourt case par case, waypoint

        return motor_.remaining_distance() <= 0.001f
                   ? Status::kSuccess
                   : Status::kRunning;
    }

    Status Npc::Locked(){
        std::println("locked");
        return Status::kRunning;
    }
} // namespace api::ai
