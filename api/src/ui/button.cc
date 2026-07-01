//
// Created by noahs on 25.06.2026.
//
#include "ui/button.h"

#include <SFML/Graphics/Texture.hpp>


namespace api::ui {
std::span<sf::Vertex> Button::GetVertices(){

  if (!visible_) return {};

  vertices_[0] = sf::Vertex(pos_, kColor, tiling_.position);
  vertices_[1] = sf::Vertex(pos_ + sf::Vector2f({vertex_size_.x, 0.f}), kColor,
                            tiling_.position + sf::Vector2f(tiling_.size.x, 0.f)); // + X
  vertices_[2] = sf::Vertex(pos_ + vertex_size_, kColor, tiling_.position + tiling_.size);
  vertices_[3] = sf::Vertex(pos_, kColor, tiling_.position);
  vertices_[4] = sf::Vertex(pos_ + vertex_size_, kColor, tiling_.position + tiling_.size);
  vertices_[5] = sf::Vertex(pos_ + sf::Vector2f(0.f, vertex_size_.y), kColor,
                            tiling_.position + sf::Vector2f(0.f, tiling_.size.y)); // + Y

  return std::span<sf::Vertex>(vertices_);

}

// ReSharper disable once CppConstValueFunctionReturnType
std::string Button::GetLabel() const{
  return text_;
}

// ReSharper disable once CppConstValueFunctionReturnType
sf::Vector2f Button::GetPosition() const{
  return pos_;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
sf::Vector2f Button::GetVertexSize() const{
  return vertex_size_;
}

void Button::OnHoverEnter(){
  if (hoverTiling_ != kNoTiling) tiling_ = hoverTiling_;
  if (hoverCallback_) hoverCallback_();
}

void Button::OnHoverExit(){
  tiling_ = basicTiling_;
}

void Button::OnPressedLeft(){
  if (leftClickTiling_ != kNoTiling) tiling_ = leftClickTiling_;
  if (leftClickCallback_) leftClickCallback_();
}

void Button::OnReleasedLeft(){
  tiling_ = basicTiling_;
}


} // namespace api::ui
