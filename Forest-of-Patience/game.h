#pragma once
#include <SFML/Graphics.hpp>

#define GRAVITY_ACC 9.81
#define FRICTION 0.15
#define WINDOW_WIDTH 1131
#define WINDOW_HEIGHT 707
#define MAX_HP 20

using namespace sf;
struct point
{
  int x, y;
};

class Obstacle : public Sprite {
  // 장애물
public:
  Obstacle();
  explicit Obstacle(const Texture&, double x, double y);
  explicit Obstacle(const Texture& t, IntRect r);
  ~Obstacle();

private:

};

class GameCharacter : public Sprite {
  //캐릭터
public:
  GameCharacter();
  explicit GameCharacter(const Texture&);
  ~GameCharacter();

  void stand();
  void jump();
  void fall(bool, double);

  void moveRight();
  void moveLeft();

  void climbUp(const point[2]);
  void climbDown(const point[2]);


  void increaseHp();
  void decreaseHp();

  int getHp() { return this->_hp; }
  void setHp(int hp) { this->_hp = hp; }

  bool onHit(const Sprite&);
  void damagedBy(Obstacle&, bool);
  bool onBlock(const point&);
  bool onBlock(const Sprite&);
  bool onMoving();
  bool onClimbing() { return _onClimbing; }

  bool canClimbing(const point rope[2]);
  bool canClimbing(const Sprite&);
  void gravity(bool);

  double getDy() { return this->_velocity.y; }
  double getDx() { return this->_velocity.x; }
  void setDx(double dx) { this->_velocity.x = dx; }
  void setDy(double dy) { this->_velocity.y = dy; }
  void setClimbing(bool b) { _onClimbing = b; }

private:
  // Vector2i _source; // sprite sheet crop
  bool _onClimbing; // 로프를 타고 있는 중인지
  unsigned int _hp;
  Vector2f _velocity;

};

class Rope : public Sprite {
public:
  Rope();
  explicit Rope(const Texture&);
  ~Rope();

private:

};



class Spear : public Obstacle
{
public:
  Spear();
  explicit Spear(const Texture&, double x, double y);
  ~Spear();
  //void attack(const Texture&);
private:


};
class Banana : public Obstacle
{
public:
  Banana();
  explicit Banana(const Texture&, double, double);
  ~Banana();
  bool threw(bool, float);
private:
  float _time;
};

class Monkey : public Obstacle
{
public:
  Banana banana;

  Monkey();
  explicit Monkey(const Texture& monkey, const Texture& banana, const IntRect& r, bool is_left);
  explicit Monkey(const Texture& monkey, const Texture& banana, double x, double y, bool is_left);
  ~Monkey();
  //double getPosition_x(){ return this->_source.x; }
  //double getPosition_y(){ return this->_source.y; }
  bool throw_banana(float time);
private:
  unsigned int minus_hp;
  
  float _t_att;
  bool is_left;
  bool _action;

};


class Floor {
  // 바닥

private:

};