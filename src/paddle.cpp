#include "paddle.h"

void Paddle::moveLeft() {
    m_pos_x -= speed;
}

void Paddle::moveRight() {
    m_pos_x += speed;
}

float Paddle::getLength() const {
    return m_length;
}

float Paddle::getHeight() const {
    return m_height;
}

float Paddle::getPosX() const {
    return m_pos_x;
}

float Paddle::getPosY() const {
    return m_pos_y;
}