//
// Created by sebas on 10.06.2026.
//

#ifndef CITYBUILDER_NPC_MANAGER_H
#define CITYBUILDER_NPC_MANAGER_H
#include <mdspan>
#include <SFML/Graphics/RenderWindow.hpp>
#include <string_view>

#include "a_star_graph.h"
#include "npc.h"
#include "tiles/tile.h"

namespace api::ai {
class NPCManager {

  std::vector<std::unique_ptr<Npc>> npcs_;
  sf::Vector2i world_size_;
  std::string sprite_path_;
  std::unique_ptr<sf::Texture> texture_ = std::make_unique<sf::Texture>();

public:
  void Setup(std::string_view, sf::Vector2i);
  void Update(float dt);
  void Draw(sf::RenderWindow &window);
  void SpawnNPC(AStarGraph& graph);
};
}

#endif //CITYBUILDER_NPC_MANAGER_H
