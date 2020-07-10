#include "ball.h"
#include <cmath>
#include <iostream>
#include "paddle.h"

#define Pi 3.1415926f

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

bool Ball::Move(Paddle& paddle)
{
    bool hit = false;

    m_pos_x += m_speed*m_dir_x;
    m_pos_y += m_speed*m_dir_y;

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
