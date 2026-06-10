//
// Created by noahs on 01.06.2026.
//

#include "loader.h"
#include "tilemap.h"
#include "game_types.h"
#include <sstream>
#include <string>

void Loader::open(std::filesystem::path& path) {
    ifs_.open(path);
}

void Loader::close() {
    ifs_.close();
}

void Loader::visit(std::filesystem::path& path, Tilemap& tilemap) {
    open(path);

    std::string line;
    if (!std::getline(ifs_, line)) return;

    std::stringstream ss(line);
    std::string field;
    std::getline(ss, field, ';'); int terrainCount  = std::stoi(field);
    std::getline(ss, field, ';'); int resourceCount = std::stoi(field);

    tilemap.terrain_.clear();
    tilemap.resources_.clear();

    for (int i = 0; i < terrainCount && std::getline(ifs_, line); ++i) {
        std::stringstream ts(line);
        std::getline(ts, field, ';'); float x    = std::stof(field);
        std::getline(ts, field, ';'); float y    = std::stof(field);
        std::getline(ts, field, ';'); int   type = std::stoi(field);

        tilemap.terrain_.emplace_back(
            tiles::Tile<TerrainTile>{{x, y}, static_cast<TerrainTile>(type)});
    }

    for (int i = 0; i < resourceCount && std::getline(ifs_, line); ++i) {
        std::stringstream rs(line);
        std::getline(rs, field, ';'); float x    = std::stof(field);
        std::getline(rs, field, ';'); float y    = std::stof(field);
        std::getline(rs, field, ';'); int   type = std::stoi(field);

        tilemap.resources_.emplace_back(
            tiles::Tile<ResourceTile>{{x, y}, static_cast<ResourceTile>(type)});
    }

    close();
}
