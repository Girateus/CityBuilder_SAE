//
// Created by noahs on 25.06.2026.
//

#ifndef CITYBUILDER_BUTTON_H
#define CITYBUILDER_BUTTON_H

#include <span>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "clickable.h"

namespace api::ui {
    class Button : public Clickable {

      friend class ButtonMaker;
      static constexpr sf::FloatRect kNoTiling = { {-1, -1 }, {-1, -1}};
      static constexpr sf::Color kColor = sf::Color::White;

      sf::Vector2f vertex_size_;
      std::array<sf::Vertex, 6> vertices_;

      sf::Vector2f pos_;
      std::string text_;

      sf::FloatRect tiling_ = kNoTiling;
      sf::FloatRect basicTiling_ = kNoTiling;
      sf::FloatRect hoverTiling_ = kNoTiling;
      sf::FloatRect leftClickTiling_ = kNoTiling;

      UICallback hoverCallback_;
      UICallback leftClickCallback_;

      bool visible_ = true;

    protected:
        void OnHoverEnter() override;
        void OnHoverExit() override;
        void OnPressedLeft() override;
        void OnReleasedLeft() override;

    public:
        Button() = default;

        [[nodiscard]] std::span<sf::Vertex> GetVertices();
        [[nodiscard]] std::string GetLabel() const;
        [[nodiscard]] sf::Vector2f GetPosition() const;
        [[nodiscard]] sf::Vector2f GetVertexSize() const;

        void SetVisible(bool v) { visible_ = v; }
        bool IsVisible()  const { return visible_; }
    };
}

#endif  // CITYBUILDER_BUTTON_H
