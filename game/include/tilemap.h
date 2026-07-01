#ifndef CITYBUILDER_TILEMAP_H
#define CITYBUILDER_TILEMAP_H

#include <filesystem>
#include <vector>

#include "FastNoiseLite.h"
#include "ai/a_star_graph.h"
#include "game_types.h"
#include "graphics/tilemap_renderer.h"
#include "graphics/tilesheet.h"
#include "tiles/tile.h"

class Loader;
class Saver;

class Tilemap {

  sf::Vector2i grid_size_;
  sf::Vector2f grid_offset_;

  api::graphics::TilemapRenderer terrain_renderer_;
  api::graphics::Tilesheet<TerrainTile> terrain_tilesheet_;

  api::graphics::TilemapRenderer ressources_renderer_;
  api::graphics::Tilesheet<ResourceTile> ressources_tilesheet_;

  std::vector<api::tiles::Tile<TerrainTile>>   terrain_;
  std::vector<api::tiles::Tile<ResourceTile>>  resources_;

  friend class Saver;
  friend class Loader;

  void BuildRenderers(sf::Vector2f gridOffset, api::ai::AStarGraph& astar_graph);

public:
  void Setup(sf::Vector2i gridSize, sf::Vector2f gridOffset,
             api::ai::AStarGraph& astar_graph);

  void Draw(sf::RenderWindow& window);

  void Save(std::filesystem::path path, Saver& saver);
  void Load(std::filesystem::path path, Loader& loader,
            sf::Vector2f gridOffset, api::ai::AStarGraph& astar_graph);
};

#endif  // CITYBUILDER_TILEMAP_H