//
// Created by noahs on 25.06.2026.
//
#include "ui/button_maker.h"
#include "ui/button.h"

namespace api::ui {
ButtonMaker &ButtonMaker::New(){
  product = Button();
  return *this;
}

ButtonMaker &ButtonMaker::WithPosition(sf::Vector2f pos, sf::Vector2f vertexSize){
  product.pos_ = pos;
  product.vertex_size_ = vertexSize;
  product.SetZone(sf::IntRect(sf::Vector2i{pos}, sf::Vector2i{vertexSize}));

  return *this;
}

ButtonMaker &ButtonMaker::WithText(std::string text){
  product.text_ = text;
  return *this;
}

ButtonMaker &ButtonMaker::WithBaseTile(sf::FloatRect baseTiling){
  product.basicTiling_ = baseTiling;
  product.tiling_ = baseTiling;
  return *this;
}

ButtonMaker &ButtonMaker::WithHoverTile(sf::FloatRect hoverTiling){
  product.hoverTiling_ = hoverTiling;
  return *this;
}

ButtonMaker &ButtonMaker::WithClickTile(sf::FloatRect clickTiling){
  product.leftClickTiling_ = clickTiling;
  return *this;
}

ButtonMaker &ButtonMaker::WithHoverCallback(const UICallback &hCallback){
  product.hoverCallback_ = hCallback;
  return *this;
}

ButtonMaker &ButtonMaker::WithClickCallback(const UICallback &cCallback){
  product.leftClickCallback_ = cCallback;
  return *this;
}

std::unique_ptr<Button> ButtonMaker::Build(){
  return std::make_unique<Button>(product);
}

}
