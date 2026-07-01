//
// Created by sebas on 20.05.2026.
//

#ifndef CITYBUILDER_TILEMAP_GENERATOR_H
#define CITYBUILDER_TILEMAP_GENERATOR_H

#include <array>
#include <random>
#include <ranges>
#include <span>

#include "tile.h"
//ToDo: Perlin Noise for terrain tile.
namespace api::tiles::generator {

inline std::vector<Tile<TerrainTile>> GenerateTerrain(sf::Vector2i size,
                                                       sf::Vector2f offset) {
    std::vector<Tile<TerrainTile>> terrainMap;

    FastNoiseLite terrain_noise;
    terrain_noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    terrain_noise.SetSeed(1337);
    terrain_noise.SetFrequency(0.0025f);

    FastNoiseLite grass_noise;
    grass_noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    grass_noise.SetSeed(9999);
    grass_noise.SetFrequency(0.008f);

    // size.x = nombre de tiles → limite en pixels = size.x * offset.x
    const float limit_x = static_cast<float>(size.x) * offset.x;
    const float limit_y = static_cast<float>(size.y) * offset.y;

  int countA = 0, countB = 0, countC = 0, countD = 0, countWater = 0;
  float minG = 1.f, maxG = -1.f;

    for (float x = 0.f; x < limit_x; x += offset.x) { // NOLINT(*-flp30-c)
        for (float y = 0.f; y < limit_y; y += offset.y) { // NOLINT(*-flp30-c)
            if (std::abs(terrain_noise.GetNoise(x, y)) <= 0.3f) {

              const float g = grass_noise.GetNoise(x, y);

              if (g < minG) minG = g;
              if (g > maxG) maxG = g;

              TerrainTile grass_type;
              if (g > 0.5f) grass_type = TerrainTile::kGrassA;
              else if (g > 0.2f) grass_type = TerrainTile::kGrassB;
              else if (g > -0.1f)grass_type = TerrainTile::kGrassC;
              else grass_type = TerrainTile::kGrassD;

              terrainMap.emplace_back(Tile<TerrainTile>{{{x, y}, true},  grass_type});
            } else {
                terrainMap.emplace_back(Tile<TerrainTile>{{{x, y}, false}, TerrainTile::kWaterA});
            }
        }
    }

    return terrainMap;
}

inline std::vector<Tile<ResourceTile>> SeedAndGrow(
    std::span<Tile<TerrainTile>> terrain) {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution rnd(0.f, 1.f);

    constexpr std::array<std::pair<ResourceTile, float>, 3> seeds{{
        {ResourceTile::kWood, 0.25f},
        {ResourceTile::kRock, 0.15f},
        {ResourceTile::kFood, 0.10f},
    }};

    auto map = terrain
        | std::views::filter([](auto tile) { return tile.type == TerrainTile::kGrassA || tile.type == TerrainTile::kGrassB || tile.type == TerrainTile::kGrassC || tile.type == TerrainTile::kGrassD; })
        | std::views::transform([&](auto tile) {
            std::vector<Tile<ResourceTile>> result;
            for (auto& [type, prob] : seeds) {
                if (rnd(gen) <= prob) {
                    result.emplace_back(Tile<ResourceTile>{{tile.Pos, false}, type});
                    break;
                }
            }
            return result;
        })
        | std::views::join;

    std::vector<Tile<ResourceTile>> ressourceMap;
    for (auto tile : map) {
        ressourceMap.emplace_back(tile);
    }
    return ressourceMap;
}

}; // namespace api::tiles::generator

#endif //CITYBUILDER_TILEMAP_GENERATOR_H