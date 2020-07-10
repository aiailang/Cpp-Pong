#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "ball.h"
#include "paddle.h"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();


  void Render(std::vector<std::shared_ptr<Ball>>& _balls, Paddle const paddle);
  void UpdateWindowTitle(int score, int fps);

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  SDL_Texture* ball_tex;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif