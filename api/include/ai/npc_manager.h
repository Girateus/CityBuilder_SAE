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
//#include "game_types.h"
#include "houses/house_type.h"
#include "resources/resource_type.h"

namespace api::ai {
class NPCManager {

  std::vector<std::unique_ptr<Npc>> npcs_;
  sf::Vector2i world_size_;

  std::unique_ptr<sf::Texture> texture_lumberjack_ = std::make_unique<sf::Texture>();
  std::unique_ptr<sf::Texture> texture_gatherer_   = std::make_unique<sf::Texture>();
  std::unique_ptr<sf::Texture> texture_miner_      = std::make_unique<sf::Texture>();



public:
   void Setup(std::string_view lumberjack_path,
               std::string_view gatherer_path,
               std::string_view miner_path,
               sf::Vector2i world_size);

  void Update(float dt);
  void Draw(sf::RenderWindow &window);
  using FindResourceFn   = std::function<std::optional<sf::Vector2f>(sf::Vector2f, ResourceTile)>;
  using RemoveResourceFn = std::function<void(sf::Vector2f)>;
  using UnreserveFn      = std::function<void(sf::Vector2f)>;
  void SpawnNPC(AStarGraph& graph, HouseTile type, sf::Vector2f house_pos,
              FindResourceFn find_fn, RemoveResourceFn remove_fn,  UnreserveFn unreserve_fn);


};
}

#endif //CITYBUILDER_NPC_MANAGER_H
