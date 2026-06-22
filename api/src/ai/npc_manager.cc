//
// Created by sebas on 10.06.2026.
//

#include "ai/npc_manager.h"

#include <mdspan>

#include "ai/a_star_graph.h"
#include "rng/rng.h"

namespace api::ai {
void NPCManager::Setup(std::string_view sprite_path, sf::Vector2i world_size){
  world_size_ = world_size;
  sprite_path_ = sprite_path;
  if (texture_->loadFromFile(std::string(sprite_path))) {
  }
  //npcs_.reserve(10);

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

void NPCManager::SpawnNPC(AStarGraph& graph){
  npcs_.emplace_back(std::make_unique<Npc>());
  npcs_.back()->Setup(texture_.get(), world_size_, graph.GetRandomNode(), graph);
}
}
