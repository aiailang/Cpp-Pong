#ifndef PADDLE_H
#define PADDLE_H

class Paddle {
 public:
   // construtor / destructor
   Paddle(float length, float height, float xpos, float ypos) : 
      m_length(length), 
      m_height(height),
      m_pos_x(xpos),
      m_pos_y(ypos) {}

   // Getter and Setter
   float getLength() const;
   float getHeight() const;

   float getPosX() const;
   float getPosY() const;

   // 
   void moveLeft();
   void moveRight();

 private:
   float m_length;
   float m_height;

   float speed{1.0f};
   float m_pos_x;
   float m_pos_y;
};

#endif