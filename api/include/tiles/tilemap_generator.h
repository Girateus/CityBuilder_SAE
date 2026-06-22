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

namespace api::tiles::generator {

inline std::vector<Tile<TerrainTile>> GenerateTerrain(sf::Vector2i size,
                                                       sf::Vector2f offset) {
  std::vector<Tile<TerrainTile>> terrainMap;

  FastNoiseLite noise;
  noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
  noise.SetSeed(1337);
  noise.SetFrequency(0.0025f);

  // size.x = nombre de tiles → limite en pixels = size.x * offset.x
  const float limit_x = static_cast<float>(size.x) * offset.x;
  const float limit_y = static_cast<float>(size.y) * offset.y;

  for (float x = 0.f; x < limit_x; x += offset.x) { // NOLINT(*-flp30-c)
    for (float y = 0.f; y < limit_y; y += offset.y) { // NOLINT(*-flp30-c)
      if (std::abs(noise.GetNoise(x, y)) <= 0.3f) {
        terrainMap.emplace_back(Tile<TerrainTile>{{{x, y}, true},  TerrainTile::kGrassA});
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
        | std::views::filter([](auto tile) { return tile.type == TerrainTile::kGrassA; })
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