#ifndef GAME_H
#define GAME_H

#include <random>
#include <vector>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "ball.h"
#include "paddle.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;

 private:
  std::vector<std::shared_ptr<Ball>> _balls;
  Ball ball;
  Paddle paddle;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  
  int score{0};

  void PlaceBall(std::shared_ptr<Ball> ball);
  void Update();
};

#endif