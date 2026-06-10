//
// Created by sebas on 20.05.2026.
//

#include "tilemap.h"

#include "game_types.h"
#include "saver.h"
#include "loader.h"
#include "tiles/tilemap_generator.h"

void Tilemap::BuildRenderers(sf::Vector2f gridOffset) {

    if (terrain_tilesheet_.InitTileSheet("_assets/tiles/RTS_medieval@2_no_margins_transparent.png", 128)) {
        terrain_tilesheet_.AddTile(TerrainTile::kGrassA, 0, 0);
        terrain_tilesheet_.AddTile(TerrainTile::kGrassB, 1, 0);
        terrain_tilesheet_.AddTile(TerrainTile::kWaterA, 0, 2);
        terrain_tilesheet_.AddTile(TerrainTile::kWaterB, 1, 2);

        terrain_renderer_.SetTexture(terrain_tilesheet_.GetTexture());
        terrain_renderer_.ClearVertices();

        for (auto& tile : terrain_) {
            terrain_renderer_.AddTile(tile.pos, gridOffset, terrain_tilesheet_.GetBounds(tile.type));
        }
    }

    if (ressources_tilesheet_.InitTileSheet("_assets/tiles/RTS_medieval@2_no_margins_transparent.png", 128)) {
        ressources_tilesheet_.AddTile(ResourceTile::kWood, 5, 3);
        ressources_tilesheet_.AddTile(ResourceTile::kRock, 5, 4);
        ressources_tilesheet_.AddTile(ResourceTile::kFood, 10, 5);

        ressources_renderer_.SetTexture(ressources_tilesheet_.GetTexture());
        ressources_renderer_.ClearVertices();

        for (auto& tile : resources_) {
            ressources_renderer_.AddTile(tile.pos, gridOffset, ressources_tilesheet_.GetBounds(tile.type));
        }
    }
}

void Tilemap::Setup(sf::Vector2f gridSize, sf::Vector2f gridOffset) {
    terrain_  = tiles::generator::GenerateTerrain(gridSize, gridOffset);
    resources_ = tiles::generator::SeedAndGrow(terrain_);

    BuildRenderers(gridOffset);
}

void Tilemap::Draw(sf::RenderWindow& window) {
    terrain_renderer_.Draw(window);
    ressources_renderer_.Draw(window);
}

void Tilemap::Save(std::filesystem::path path, Saver& saver) {
    saver.visit(path, *this);
}

void Tilemap::Load(std::filesystem::path path, Loader& loader, sf::Vector2f gridOffset) {
    loader.visit(path, *this);
    BuildRenderers(gridOffset);
}
