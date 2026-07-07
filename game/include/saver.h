//
// Created by noahs on 01.06.2026.
//

#ifndef CITYBUILDER_SAVER_H
#define CITYBUILDER_SAVER_H

#include <filesystem>
#include <fstream>

#include "serializer_visitor.h"

class Saver : public SerializerVisitor {
  std::ofstream ofs_;


  void open(std::filesystem::path& path) override;
  void close() override;

 public:
  void visit(std::filesystem::path& path, Tilemap& tilemap) override;
};

#endif  // CITYBUILDER_SAVER_H
