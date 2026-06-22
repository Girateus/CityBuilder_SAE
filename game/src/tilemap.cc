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

    if (terrain_tilesheet_.InitTileSheet("_assets/tiles/RTS_medieval@2_no_margins_transparent.png", 128)) {
        terrain_tilesheet_.AddTile(TerrainTile::kGrassA, 0, 0);
        terrain_tilesheet_.AddTile(TerrainTile::kGrassB, 1, 0);
        terrain_tilesheet_.AddTile(TerrainTile::kWaterA, 0, 2);
        terrain_tilesheet_.AddTile(TerrainTile::kWaterB, 1, 2);

        terrain_renderer_.SetTexture(terrain_tilesheet_.GetTexture());
        terrain_renderer_.ClearVertices();

        for (auto& tile : terrain_) {
            // Seules les cases herbe sont walkables (prof : kGrassA et kGrassB)
            if (tile.type == TerrainTile::kGrassA || tile.type == TerrainTile::kGrassB) {
                astar_graph.AddNode(sf::Vector2i{tile.Pos});
            }
            terrain_renderer_.AddTile(tile.Pos, gridOffset, terrain_tilesheet_.GetBounds(tile.type));
        }
    }

    if (ressources_tilesheet_.InitTileSheet("_assets/tiles/RTS_medieval@2_no_margins_transparent.png", 128)) {
        ressources_tilesheet_.AddTile(ResourceTile::kWood, 5, 3);
        ressources_tilesheet_.AddTile(ResourceTile::kRock, 5, 4);
        ressources_tilesheet_.AddTile(ResourceTile::kFood, 5, 5); // prof : 5 pas 10

        ressources_renderer_.SetTexture(ressources_tilesheet_.GetTexture());
        ressources_renderer_.ClearVertices();

        for (auto& tile : resources_) {
            // Une ressource bloque la case — on la retire du graphe (prof)
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