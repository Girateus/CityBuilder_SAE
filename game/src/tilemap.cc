//
// Created by sebas on 20.05.2026.
//
#include "tilemap.h"

#include "ai/a_star_graph.h"
#include "game_types.h"
#include "loader.h"
#include "saver.h"
#include "tiles/tilemap_generator.h"

void Tilemap::BuildRenderers(sf::Vector2f gridOffset, api::ai::AStarGraph& astar_graph) {

    if (terrain_tilesheet_.InitTileSheet("_assets/tiles/tilesheet.png", 240)) {
        terrain_tilesheet_.AddTile(TerrainTile::kGrassA, 0, 2);
        terrain_tilesheet_.AddTile(TerrainTile::kGrassB, 2, 2);
        terrain_tilesheet_.AddTile(TerrainTile::kGrassC, 3, 2);
        terrain_tilesheet_.AddTile(TerrainTile::kGrassD, 5, 1);
        terrain_tilesheet_.AddTile(TerrainTile::kWaterA, 0, 0);
        terrain_tilesheet_.AddTile(TerrainTile::kWaterB, 1, 0);

        terrain_renderer_.SetTexture(terrain_tilesheet_.GetTexture());
        terrain_renderer_.ClearVertices();

        for (auto& tile : terrain_) {
          if (tile.type == TerrainTile::kGrassA || tile.type == TerrainTile::kGrassB || tile.type == TerrainTile::kGrassC || tile.type == TerrainTile::kGrassD) {
                astar_graph.AddNode(sf::Vector2i{tile.Pos});
            }
            terrain_renderer_.AddTile(tile.Pos, gridOffset, terrain_tilesheet_.GetBounds(tile.type));
        }
    }

    if (ressources_tilesheet_.InitTileSheet("_assets/tiles/tilesheet.png", 240)) {
        ressources_tilesheet_.AddTile(ResourceTile::kWood, 1, 1);
        ressources_tilesheet_.AddTile(ResourceTile::kRock, 0, 1);
        ressources_tilesheet_.AddTile(ResourceTile::kFood, 2, 1);

        ressources_renderer_.SetTexture(ressources_tilesheet_.GetTexture());
        ressources_renderer_.ClearVertices();

        for (auto& tile : resources_) {
            astar_graph.RemoveNode(sf::Vector2i{tile.Pos});
            ressources_renderer_.AddTile(tile.Pos, gridOffset, ressources_tilesheet_.GetBounds(tile.type));
        }
    }

    if (house_tilesheet_.InitTileSheet("_assets/tiles/tilesheet.png", 240)) {
      house_tilesheet_.AddTile(HouseTile::kGatherer, 2, 3);
      house_tilesheet_.AddTile(HouseTile::kMiner, 0, 3);
      house_tilesheet_.AddTile(HouseTile::kLumberjack, 1, 3);

      house_renderer_.SetTexture(house_tilesheet_.GetTexture());
    }
}

void Tilemap::Setup(sf::Vector2i gridSize, sf::Vector2f gridOffset, api::ai::AStarGraph& astar_graph) {
    grid_size_   = gridSize;
    grid_offset_ = gridOffset;

    terrain_   = api::tiles::generator::GenerateTerrain(grid_size_, grid_offset_);
    resources_ = api::tiles::generator::SeedAndGrow(terrain_); // multi-seeds, pas de _seed

    BuildRenderers(grid_offset_, astar_graph);
}

bool Tilemap::IsWalkable(sf::Vector2f world_pos) const {
  for (auto& tile : terrain_) {
    if (tile.Pos.x == std::floor(world_pos.x / grid_offset_.x) * grid_offset_.x &&
        tile.Pos.y == std::floor(world_pos.y / grid_offset_.y) * grid_offset_.y) {
      return tile.IsWalkable;
        }
  }
  return false;
}

void Tilemap::PlaceHouse(sf::Vector2f world_pos, HouseTile type) {
  const float tx = std::floor(world_pos.x / grid_offset_.x) * grid_offset_.x;
  const float ty = std::floor(world_pos.y / grid_offset_.y) * grid_offset_.y;

  houses_.emplace_back(api::tiles::Tile<HouseTile>{{{tx, ty}, false}, type});

  // Rebuild house renderer
  house_renderer_.ClearVertices();
  for (auto& h : houses_) {
    house_renderer_.AddTile(h.Pos, grid_offset_, house_tilesheet_.GetBounds(h.type));
  }
}

void Tilemap::Draw(sf::RenderWindow& window) {
    terrain_renderer_.Draw(window);
    ressources_renderer_.Draw(window);
    house_renderer_.Draw(window);
}

void Tilemap::Save(std::filesystem::path path, Saver& saver) {
    saver.visit(path, *this);
}

void Tilemap::Load(std::filesystem::path path, Loader& loader, sf::Vector2f gridOffset,
                   api::ai::AStarGraph& astar_graph) {
    loader.visit(path, *this);
    BuildRenderers(gridOffset, astar_graph);
}