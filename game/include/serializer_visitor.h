//
// Created by noahs on 01.06.2026.
//

#ifndef CITYBUILDER_SERIALIZER_VISITOR_H
#define CITYBUILDER_SERIALIZER_VISITOR_H

#include <filesystem>

class Tilemap;

class SerializerVisitor {
  virtual void open(std::filesystem::path&) = 0;
  virtual void close() = 0;

 public:
  virtual ~SerializerVisitor() = default;
  virtual void visit(std::filesystem::path&, Tilemap&) = 0;
};

#endif  // CITYBUILDER_SERIALIZER_VISITOR_H
