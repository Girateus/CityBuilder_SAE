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
}

void Tilemap::Setup(sf::Vector2i gridSize, sf::Vector2f gridOffset, api::ai::AStarGraph& astar_graph) {
    grid_size_   = gridSize;
    grid_offset_ = gridOffset;

    terrain_   = api::tiles::generator::GenerateTerrain(grid_size_, grid_offset_);
    resources_ = api::tiles::generator::SeedAndGrow(terrain_); // multi-seeds, pas de _seed

    BuildRenderers(grid_offset_, astar_graph);
}

void Tilemap::Draw(sf::RenderWindow& window) {
    terrain_renderer_.Draw(window);
    ressources_renderer_.Draw(window);
}

void Tilemap::Save(std::filesystem::path path, Saver& saver) {
    saver.visit(path, *this);
}

void Tilemap::Load(std::filesystem::path path, Loader& loader, sf::Vector2f gridOffset,
                   api::ai::AStarGraph& astar_graph) {
    loader.visit(path, *this);
    BuildRenderers(gridOffset, astar_graph);
}