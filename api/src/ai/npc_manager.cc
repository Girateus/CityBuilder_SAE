//
// Created by sebas on 10.06.2026.
//

#include "ai/npc_manager.h"

#include <mdspan>

#include "ai/a_star_graph.h"
#include "rng/rng.h"

namespace api::ai {
void NPCManager::Setup(std::string_view lumberjack_path,
                       std::string_view gatherer_path,
                       std::string_view miner_path,
                       sf::Vector2i world_size) {
  world_size_ = world_size;
  texture_lumberjack_->loadFromFile(std::string(lumberjack_path));
  texture_gatherer_->loadFromFile(std::string(gatherer_path));
  texture_miner_->loadFromFile(std::string(miner_path));
}

void NPCManager::Update(float dt){
  for (auto &npc: npcs_) {
    npc->Update(dt);
  }
}

void NPCManager::Draw(sf::RenderWindow &window){
  for (auto &npc: npcs_) {
    npc->Draw(window);
  }
}

void NPCManager::SpawnNPC(AStarGraph& graph, HouseTile type, sf::Vector2f house_pos) {
  sf::Texture* tex = nullptr;
  switch (type) {
    case HouseTile::kLumberjack: tex = texture_lumberjack_.get(); break;
    case HouseTile::kGatherer:   tex = texture_gatherer_.get();   break;
    case HouseTile::kMiner:      tex = texture_miner_.get();      break;
  }

  if (!tex) return;

  const sf::Vector2i start{
    static_cast<int>(house_pos.x),
    static_cast<int>(house_pos.y)
};

  npcs_.emplace_back(std::make_unique<Npc>());
  npcs_.back()->Setup(tex, world_size_, start, graph);
}
}
