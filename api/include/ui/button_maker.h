//
// Created by noahs on 25.06.2026.
//

#ifndef CITYBUILDER_BUTTON_MAKER_H
#define CITYBUILDER_BUTTON_MAKER_H

#include "clickable.h"
#include "button.h"

namespace api::ui {
    class ButtonMaker {
      Button product;
    public:
      ButtonMaker &New();
      ButtonMaker &WithPosition(sf::Vector2f pos, sf::Vector2f vertexSize);
      ButtonMaker &WithText(std::string text);

      ButtonMaker &WithBaseTile(sf::FloatRect baseTiling);
      ButtonMaker &WithHoverTile(sf::FloatRect baseTiling);
      ButtonMaker &WithClickTile(sf::FloatRect clickTiling);

      ButtonMaker &WithHoverCallback(const UICallback &hCallback);

      ButtonMaker &WithClickCallback(const UICallback &hCallback);

      std::unique_ptr<Button> Build();

    };
}

#endif  // CITYBUILDER_BUTTON_MAKER_H
