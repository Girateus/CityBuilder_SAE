//
// Created by noahs on 25.06.2026.
//

#ifndef CITYBUILDER_CLICKABLE_H
#define CITYBUILDER_CLICKABLE_H

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Window/Event.hpp>
#include <functional>

namespace api::ui {

    using UICallback = std::function<void()>;

    class Clickable {
    private:
        sf::IntRect zone_;
        bool isHover_ = false;

    protected:
      Clickable() = default;

      virtual void OnHoverEnter() {}
      virtual void OnHoverExit() {}
      virtual void OnReleasedRight() {}
      virtual void OnReleasedLeft() {}
      virtual void OnPressedLeft() {}
      virtual void OnPressedRight() {}

    public:
      virtual ~Clickable() = default;
      bool DoHoverEnterEvents(const std::optional<sf::Event> &event);
      bool DoHoverExitEvents(const std::optional<sf::Event> &event);
      bool DoReleasedEvents(const std::optional<sf::Event> &event);
      bool DoPressedEvents(const std::optional<sf::Event> &event);
      bool HandleEvent(const std::optional<sf::Event> &event);

      void LeaveHover();

      virtual void SetZone(sf::IntRect zone);

    };

}

#endif  // CITYBUILDER_CLICKABLE_H
