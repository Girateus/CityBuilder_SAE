//
// Created by sebas on 20.05.2026.
//

#ifndef CITYBUILDER_TILEMAP_GENERATOR_H
#define CITYBUILDER_TILEMAP_GENERATOR_H

//#include <__msvc_ranges_to.hpp>
#include <random>
#include <ranges>
#include <span>

#include "tile.h"

namespace tiles::generator {

inline std::vector<Tile<TerrainTile>> GenerateTerrain(sf::Vector2f size,
                                                       sf::Vector2f offset) {
  std::vector<Tile<TerrainTile>> terrainMap;

  FastNoiseLite noise;
  noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
  noise.SetSeed(1337);
  noise.SetFrequency(0.0025f);

  for (float x = 0.f; x < size.x; x += offset.x) {    // NOLINT(*-flp30-c)
    for (float y = 0.f; y < size.y; y += offset.y) {  // NOLINT(*-flp30-c)

      // Generator stuff -----------------------------
      if (abs(noise.GetNoise(x, y)) <= 0.3f) {
        terrainMap.emplace_back(Tile{{x, y}, TerrainTile::kGrassA});
      } else {
        terrainMap.emplace_back(Tile{{x, y}, TerrainTile::kWaterA});
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

   constexpr std::array<std::pair<ResourceTile, float>, 3> seeds = {
    {
      {ResourceTile::kWood, 0.25f},
      {ResourceTile::kRock, 0.15f},
      {ResourceTile::kFood, 0.10f},
  }
  };

  auto map = terrain | std::views::filter([](auto tile) {
               return tile.type == TerrainTile::kGrassA;
             }) |
             std::views::transform([&](auto tile) {
               std::vector<Tile<ResourceTile>> result;
               for (auto& [type, prob] : seeds) {
                 if (rnd(gen) <= prob) {
                   result.emplace_back(tile.pos, type);
                   break;
                 }
               }
               return result;
             }) |
             std::views::join;

  std::vector<Tile<ResourceTile>> ressourceMap;
  for (auto tile : map) {
    ressourceMap.emplace_back(tile);
  }
  return ressourceMap;
}

};  // namespace tiles::generator

#endif  // CITYBUILDER_TILEMAP_GENERATOR_H
