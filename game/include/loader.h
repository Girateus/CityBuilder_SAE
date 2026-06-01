//
// Created by noahs on 01.06.2026.
//

#ifndef CITYBUILDER_LOADER_H
#define CITYBUILDER_LOADER_H

#include <filesystem>
#include <fstream>

#include "serializer_visitor.h"

class Loader : public SerializerVisitor {
  std::ifstream ifs_;

  void open(std::filesystem::path& path) override;
  void close() override;

 public:
  void visit(std::filesystem::path& path, Tilemap& tilemap) override;
};

#endif  // CITYBUILDER_LOADER_H
