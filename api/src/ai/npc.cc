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
static sf::Vector2i SnapToGrid(sf::Vector2f pos, int offset) {
  return {
    (static_cast<int>(pos.x) / offset) * offset,
    (static_cast<int>(pos.y) / offset) * offset
};
}

void Npc::Setup(const sf::Texture* texture, sf::Vector2i world_size,
            sf::Vector2i start_position, AStarGraph& graph,
            sf::Vector2f house_pos, ResourceTile resource_type,
            FindResourceFn find_fn, RemoveResourceFn remove_fn, UnreserveFn unreserve_fn){
      world_size_      = world_size;
      house_pos_       = house_pos;
      resource_type_   = resource_type;
      find_resource_   = std::move(find_fn);
      remove_resource_ = std::move(remove_fn);
      unreserve_resource_ = std::move(unreserve_fn);

        if (texture != nullptr) {
            sprite_ = sf::Sprite(*texture);
          const sf::Vector2u textureSize = texture->getSize();
          sprite_->setScale({
              32.f / static_cast<float>(textureSize.x),
              32.f / static_cast<float>(textureSize.y)
          });

          /*sprite_->setOrigin({
              static_cast<float>(textureSize.x) / 2.f,
              static_cast<float>(textureSize.y) / 2.f
          });*/
        }

        //sf::Vector2f start_position_f = sf::Vector2f{static_cast<float>(start_position.x), static_cast<float>(start_position.y)};

        motor_.set_position(start_position);
        motor_.set_destination(start_position); // stay put until the first pick
        motor_.set_speed(kSpeed);

        using namespace core::ai::behaviour_tree::node_factory;

        auto harvestSequence = MakeSequence();
    harvestSequence->AddChild(MakeAction([this] { return FindResource(); }));
    harvestSequence->AddChild(MakeAction([this] { return MoveToResource(); }));
    harvestSequence->AddChild(MakeAction([this] { return Harvest(); }));
    harvestSequence->AddChild(MakeAction([this] { return ReturnHome(); }));
    harvestSequence->AddChild(MakeAction([this] { return WaitAtHome(); }));

    bt_root_ = std::move(harvestSequence);
    astar_graph_ = &graph;
}

sf::Vector2f Npc::Position() const {
  return motor_.position();
}


Status Npc::FindResource() {
  search_cooldown_ -= dt_;
  if (search_cooldown_ > 0.f) return Status::kRunning;
  search_cooldown_ = 0.5f;

  path_.SetPath({});

  auto result = find_resource_(house_pos_, resource_type_); // ← ReserveNearestResource
  if (!result.has_value()) return Status::kRunning;

  current_resource_pos_ = *result;

  constexpr int kOffset = 32;
  const sf::Vector2i snapped_start{
    (static_cast<int>(motor_.position().x) / kOffset) * kOffset,
    (static_cast<int>(motor_.position().y) / kOffset) * kOffset
};
  sf::Vector2i snapped_end{
    (static_cast<int>(current_resource_pos_.x) / kOffset) * kOffset,
    (static_cast<int>(current_resource_pos_.y) / kOffset) * kOffset
};

  if (!astar_graph_->ContainsNode(snapped_end)) {
    bool found = false;
    for (int radius = 1; radius <= 5 && !found; ++radius) {
      for (int dx = -radius; dx <= radius && !found; ++dx) {
        for (int dy = -radius; dy <= radius && !found; ++dy) {
          sf::Vector2i candidate{
            snapped_end.x + dx * kOffset,
            snapped_end.y + dy * kOffset
        };
          if (astar_graph_->ContainsNode(candidate)) {
            snapped_end = candidate;
            found = true;
          }
        }
      }
    }
    if (!found) {
      unreserve_resource_(current_resource_pos_); // ← libère si pas atteignable
      return Status::kRunning;
    }
  }

  path_.SetPath(astar_graph_->GetPath(snapped_start, snapped_end));

  if (!path_.IsValid()) {
    unreserve_resource_(current_resource_pos_); // ← libère si pas de chemin
    return Status::kRunning;
  }

  search_cooldown_ = 0.f;
  path_.NextPosition();
  motor_.set_destination(path_.CurrentPosition());
  return Status::kSuccess;
}

Status Npc::MoveToResource() {
  std::println("MoveToResource appelé | remaining: {}", motor_.remaining_distance());
  if (motor_.remaining_distance() <= 0.001f) {
    if (path_.IsGoalReached()) return Status::kSuccess;
    path_.NextPosition();
    motor_.set_destination(path_.CurrentPosition());
  }
  return Status::kRunning;
}

Status Npc::Harvest() {
    harvest_timer_ += dt_;        // ← utilise dt_ au lieu de 0.016f
    if (harvest_timer_ >= 5.f) {
        harvest_timer_ = 0.f;
        remove_resource_(current_resource_pos_);
        return Status::kSuccess;
    }
    return Status::kRunning;
}

Status Npc::ReturnHome() {
  // Calcule le chemin retour seulement au premier appel
  if (!path_.IsValid()) {
    constexpr int kOffset = 32;
    const sf::Vector2i start{
      (static_cast<int>(motor_.position().x) / kOffset) * kOffset,
      (static_cast<int>(motor_.position().y) / kOffset) * kOffset
  };
    const sf::Vector2i end{
      (static_cast<int>(house_pos_.x) / kOffset) * kOffset,
      (static_cast<int>(house_pos_.y) / kOffset) * kOffset
  };

    path_.SetPath(astar_graph_->GetPath(start, end));
    if (!path_.IsValid()) return Status::kFailure;

    path_.NextPosition();
    motor_.set_destination(path_.CurrentPosition());
  }

  if (motor_.remaining_distance() <= 0.001f) {
    if (path_.IsGoalReached()) {
      path_.SetPath({});
      return Status::kSuccess;
    }
    path_.NextPosition();
    motor_.set_destination(path_.CurrentPosition());
  }
  return Status::kRunning;
}

Status Npc::WaitAtHome() {
  home_wait_timer_ += dt_;      // ← utilise dt_ au lieu de 0.016f
  if (home_wait_timer_ >= 2.f) {
    home_wait_timer_ = 0.f;
    path_.SetPath({});
    return Status::kSuccess;
  }
  return Status::kRunning;
}


void Npc::Update(const float dt) {
  dt_ = dt;                      // ← stocker dt pour les états BT
  motor_.Update(dt);
  if (bt_root_) bt_root_->Tick();
}

void Npc::Draw(sf::RenderWindow& window) {
  if (sprite_.has_value()) {
    sprite_->setPosition(motor_.position());
    window.draw(*sprite_);
  }
}

} // namespace api::ai
