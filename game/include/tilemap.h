//
// Created by sebas on 20.05.2026.
//

#ifndef CITYBUILDER_TILEMAP_H
#define CITYBUILDER_TILEMAP_H

#include <filesystem>
#include <vector>

#include "../../api/include/tiles/tile.h"
#include "FastNoiseLite.h"
#include "game_types.h"
#include "graphics/tilemap_renderer.h"
#include "graphics/tilesheet.h"

class Loader;
class Saver;

class Tilemap {
  graphics::TilemapRenderer terrain_renderer_;
  graphics::Tilesheet<TerrainTiles> terrain_tilesheet_;

  graphics::TilemapRenderer ressources_renderer_;
  graphics::Tilesheet<RessourcesTiles> ressources_tilesheet_;

  std::vector<tiles::Tile<TerrainTiles>> terrain_;
  std::vector<tiles::Tile<RessourcesTiles>> resources_;

  friend class Saver;
  friend class Loader;

  // Reconstruit les vertex buffers depuis terrain_ et resources_
  void BuildRenderers(sf::Vector2f gridOffset);

 public:
  void Setup(sf::Vector2f gridSize, sf::Vector2f gridOffset);
  void Draw(sf::RenderWindow& window);

  void Save(std::filesystem::path path, Saver& saver);
  void Load(std::filesystem::path path, Loader& loader,
            sf::Vector2f gridOffset);
};

#endif  // CITYBUILDER_TILEMAP_H
