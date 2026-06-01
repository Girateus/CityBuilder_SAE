#include "game.h"

#include <filesystem>
#include <optional>

#include "SFML/Graphics.hpp"
#include "graphics/camera.h"
#include "graphics/tilemap_renderer.h"
#include "graphics/tilesheet.h"
#include "loader.h"
#include "saver.h"
#include "tilemap.h"

namespace game {
namespace {
constexpr sf::Vector2f world_size = {1920.f * 5, 1080.f * 5};
constexpr sf::Vector2f window_size_f = {1920.f, 1080.f};
constexpr sf::Vector2u window_size_u = {1920u, 1080u};
constexpr sf::Vector2f grid_offset = {32.f, 32.f};

const std::filesystem::path save_path = "save/map.sav";

sf::Clock clock_;
sf::RenderWindow window_;
bool isFullscreen_ = false;

Tilemap map_;
graphics::Camera camera_;

void Setup() {
  window_.create(sf::VideoMode(window_size_u), "SFML window",
                 sf::Style::Default);
  camera_.Setup(window_size_f);

  if (std::filesystem::exists(save_path)) {
    Loader loader;
    map_.Load(save_path, loader, grid_offset);
  } else {
    map_.Setup(world_size, grid_offset);

    std::filesystem::create_directories(save_path.parent_path());
    Saver saver;
    map_.Save(save_path, saver);
  }
}

void ToggleFullscreen() {
  isFullscreen_ = !isFullscreen_;
  if (isFullscreen_) {
    window_.create(sf::VideoMode::getDesktopMode(), "SFML window",
                   sf::State::Fullscreen);
  } else {
    window_.create(sf::VideoMode(window_size_u), "SFML window",
                   sf::Style::Default);
  }
  camera_.OnWindowResized(window_.getSize());
}
}  // namespace

void Loop() {
  Setup();

  while (window_.isOpen()) {
    const float dt = clock_.restart().asSeconds();

    while (const std::optional event = window_.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window_.close();
      }
      if (const auto *key = event->getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::Enter && key->alt) {
          ToggleFullscreen();
          continue;
        }

        if (key->code == sf::Keyboard::Key::F5) {
          map_.Setup(world_size, grid_offset);
          Saver saver;
          map_.Save(save_path, saver);
        }
      }
      camera_.HandleEvent(*event, window_);
    }

    camera_.Update(dt);
    camera_.Apply(window_);

    window_.clear();
    map_.Draw(window_);
    window_.display();
  }
}
}  // namespace game