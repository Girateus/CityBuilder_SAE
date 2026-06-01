//
// Created by sebas on 20.05.2026.
//

#include "tilemap.h"

#include "game_types.h"
#include "../../api/include/tiles/tilemap_generator.h"

void Tilemap::Setup(sf::Vector2f gridSize, sf::Vector2f gridOffset){
    std::vector<tiles::Tile<TerrainTiles> > terrain = tiles::generator::GenerateTerrain(gridSize, gridOffset);
    std::vector<tiles::Tile<RessourcesTiles> > resources = tiles::generator::SeedAndGrow(terrain);

    if (terrain_tilesheet_.InitTileSheet("_assets/tiles/RTS_medieval@2_no_margins_transparent.png", 128)) {
        terrain_tilesheet_.AddTile(TerrainTiles::kGrassA, 0, 0);
        terrain_tilesheet_.AddTile(TerrainTiles::kGrassB, 1, 0);
        terrain_tilesheet_.AddTile(TerrainTiles::kWaterA, 0, 2);
        terrain_tilesheet_.AddTile(TerrainTiles::kWaterB, 1, 2);


        // init textures -------------------------------------------------------------------
        terrain_renderer_.SetTexture(terrain_tilesheet_.GetTexture());
        terrain_renderer_.ClearVertices();

        for (auto &tile: terrain) {
            terrain_renderer_.AddTile(tile.pos, gridOffset, terrain_tilesheet_.GetBounds(tile.type));
        }
    }

    if (ressources_tilesheet_.InitTileSheet("_assets/tiles/RTS_medieval@2_no_margins_transparent.png", 128)) {
        ressources_tilesheet_.AddTile(RessourcesTiles::kWood, 5, 3);
        ressources_tilesheet_.AddTile(RessourcesTiles::kRock, 5, 4);
        ressources_tilesheet_.AddTile(RessourcesTiles::kFood, 5, 5);


        // init textures -------------------------------------------------------------------
        ressources_renderer_.SetTexture(ressources_tilesheet_.GetTexture());
        ressources_renderer_.ClearVertices();

        for (auto &tile: resources) {
            ressources_renderer_.AddTile(tile.pos, gridOffset, ressources_tilesheet_.GetBounds(tile.type));
        }
    }
}

void Tilemap::Draw(sf::RenderWindow &window){
    terrain_renderer_.Draw(window);
    ressources_renderer_.Draw(window);
}
