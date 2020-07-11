#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : ball(grid_width, grid_height),
      paddle(5.0f, 1.0f, grid_width/2.0f, grid_height-1.0f),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width)),
      random_h(0, static_cast<int>(grid_height)) {

  int num_ball = 2;
  for (int i = 0; i < num_ball; i++) {
    _balls.push_back(std::make_shared<Ball>(grid_width, grid_height));
    PlaceBall(_balls.at(i));
  }
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  #if 0
  for (auto ball : _balls) {
    ball->Simulate(target_frame_duration);
  }
  #endif
  #if 1
  std::for_each(_balls.begin(), _balls.end(), [=](std::shared_ptr<Ball> &ball) {
    ball->Simulate(target_frame_duration);
  });
  #endif
  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    //controller.HandleInput(running, snake);
    controller.HandleInput(running, paddle);
    Update();
    renderer.Render(_balls, paddle);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }

  int size = _balls.size();
  for (int i = 0; i < size; i++) {
    _balls.at(i)->stop();
  }
}

void Game::PlaceBall(std::shared_ptr<Ball> ball) {
  int x, y;
  x = random_w(engine);
  y = 0;

  ball->setPosition(static_cast<float>(x), static_cast<float>(y));
  ball->setDirection(0.0f, 1.0f);
}

void Game::Update() {
  for (int i = 0; i < _balls.size(); i++) {
    if (_balls.at(i)->DetectCollision(paddle)) {
      score++;
    }
  }
}

int Game::GetScore() const { return score; }