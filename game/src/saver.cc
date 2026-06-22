//
// Created by noahs on 01.06.2026.
//

#include "saver.h"

#include "tilemap.h"

void Saver::open(std::filesystem::path& path) { 
  ofs_.open(path);
}

void Saver::close() {
  ofs_.close();
}

void Saver::visit(std::filesystem::path& path, Tilemap& tilemap) {
  open(path);

  ofs_ << tilemap.terrain_.size() << ";" << tilemap.resources_.size() << "\n";

  for (auto& tile : tilemap.terrain_) {
    ofs_ << tile.Pos.x << ";" << tile.Pos.y << ";"  // ← pos → Pos
         << static_cast<int>(tile.type) << "\n";
  }

  for (auto& tile : tilemap.resources_) {
    ofs_ << tile.Pos.x << ";" << tile.Pos.y << ";"  // ← pos → Pos
         << static_cast<int>(tile.type) << "\n";
  }

  close();
}