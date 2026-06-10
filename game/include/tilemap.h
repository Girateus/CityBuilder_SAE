//
// Created by sebas on 20.05.2026.
//

#ifndef CITYBUILDER_TILEMAP_H
#define CITYBUILDER_TILEMAP_H

#include <filesystem>
#include <vector>

#include "tiles/tile.h"
#include "FastNoiseLite.h"
#include "game_types.h"
#include "graphics/tilemap_renderer.h"
#include "graphics/tilesheet.h"

class Loader;
class Saver;

class Tilemap {
  graphics::TilemapRenderer terrain_renderer_;
  graphics::Tilesheet<TerrainTile> terrain_tilesheet_;

  graphics::TilemapRenderer ressources_renderer_;
  graphics::Tilesheet<ResourceTile> ressources_tilesheet_;

  std::vector<tiles::Tile<TerrainTile>> terrain_;
  std::vector<tiles::Tile<ResourceTile>> resources_;

  friend class Saver;
  friend class Loader;

  void BuildRenderers(sf::Vector2f gridOffset);

 public:
  void Setup(sf::Vector2f gridSize, sf::Vector2f gridOffset);
  void Draw(sf::RenderWindow& window);

  void Save(std::filesystem::path path, Saver& saver);
  void Load(std::filesystem::path path, Loader& loader,
            sf::Vector2f gridOffset);
};

#endif  // CITYBUILDER_TILEMAP_H
