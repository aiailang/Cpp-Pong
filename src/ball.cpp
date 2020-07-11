#include "ball.h"
#include <cmath>
#include <iostream>
#include "SDL.h"
#include "paddle.h"

#define Pi 3.1415926f

Ball::~Ball() {
    // set up thread barrier before this object is destroyed
    std::for_each(threads.begin(), threads.end(), [](std::thread &t) {
        t.join();
    });
}

void Ball::Simulate(std::size_t target_frame_duration) {
    // launch drive function in a thread
    running = true;
    threads.emplace_back(std::thread(&Ball::Move, this, target_frame_duration));
}

void Ball::setSpeed(float init_speed)
{
    m_speed = init_speed;

}

void Ball::setDirection(float dir_x, float dir_y)
{
    m_dir_x = dir_x;
    m_dir_y = dir_y;
}

void Ball::setPosition(float x, float y) {
    m_pos_x = x;
    m_pos_y = y;
}
  
float Ball::getPositionX() const
{
    return m_pos_x;
}

float Ball::getPositionY() const
{
    return m_pos_y;
}

void Ball::stop() {
    std::lock_guard<std::mutex> lck(mtx);
    running = false;
}

void Ball::Move(std::size_t target_frame_duration)
{
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;

  while (true) {
    frame_start = SDL_GetTicks();

    std::unique_lock<std::mutex> lck(mtx);

    m_pos_x += m_speed*m_dir_x;
    m_pos_y += m_speed*m_dir_y;

    if (!running) {
        break;
    }

    lck.unlock();

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
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
}

bool Ball::DetectCollision(Paddle& paddle) {
    bool hit = false;

    std::lock_guard<std::mutex> lck(mtx);

    // hit left wall
    if (m_pos_x < 0) {
        m_dir_x = fabs(m_dir_x);
    }

    // hit right wall
    if (m_pos_x >= grid_width) {
        m_dir_x = -fabs(m_dir_x);
    }

    // hit upper wall
    if (m_pos_y < 0) {
        m_dir_y = fabs(m_dir_y);
    }

    // hit bottom wall
    if (m_pos_y >= grid_height) {
        m_dir_y = -fabs(m_dir_y);
    }

    // hit paddle
    if (m_pos_y >= paddle.getPosY()-paddle.getHeight()
        && m_pos_x >= paddle.getPosX()
        && m_pos_x <= paddle.getPosX()+paddle.getLength()
    ){
        const float MaxBounceAngle = Pi*5/12;
        // reflection angle computed based on the collision point at the board
        // https://gamedev.stackexchange.com/questions/4253/in-pong-how-do-you-calculate-the-balls-direction-when-it-bounces-off-the-paddl
        float relativeIntersectX = (paddle.getPosX()+paddle.getLength()/2.0f)-m_pos_x;
        float bounceAngle = relativeIntersectX/(paddle.getLength()/2.0f)*MaxBounceAngle;

        //printf("rel %f angle %f\n", relativeIntersectX, bounceAngle);
        //    o
        //----------------
        m_dir_x = sin(bounceAngle);
        m_dir_y = -cos(bounceAngle);
        hit = true;
    }

    return hit;
}
