#ifndef BALL_H
#define BALL_H

#include <vector>
#include <thread>
#include <mutex>

class Paddle;

struct Coord2 {
    float x;
    float y;
};

class Ball {

 public:
  // constructor and destructor
  Ball(int grid_width, int grid_height)
      : grid_width(grid_width),
        grid_height(grid_height) {}

  ~Ball();

  // Getters and Setters
  void setSpeed(float init_speed);
  void setDirection(float dir_x, float dir_y);
  void setPosition(float x, float y);

  float getPositionX() const;
  float getPositionY() const;


  void Simulate(std::size_t target_frame_duration);
  bool DetectCollision(Paddle& paddle);
  void stop();

 private:
  float m_speed{0.2f};

  float m_pos_x;
  float m_pos_y;
  float m_dir_x;
  float m_dir_y;

  int grid_width;
  int grid_height;

  bool running;
  std::mutex mtx;
  std::vector<std::thread> threads; // holds all threads that have been launched within this object

  void Move(std::size_t target_frame_duration);
};

#endif