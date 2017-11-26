#include "game.h"
#include <iostream>


GameCharacter::GameCharacter() {
  setPosition(100, 400);
}
GameCharacter::GameCharacter(const Texture& a)
{
  setTexture(a);
  setPosition(100, 400);
  _velocity = Vector2f(0.0f, 0.0f);
  _hp = 20;

  _onClimbing = false;
}
GameCharacter::~GameCharacter() {
  // default destructor
}

/////////////////////////////////////////////////
//       캐릭터가 블록의 위에 위치해 있는 지 확인    //
/////////////////////////////////////////////////
bool GameCharacter::onBlock(const point& block) {
  FloatRect r1 = this->getGlobalBounds();
  FloatRect r2(block.x, block.y + 15, 32.0f, 46.0f); // 10만큼 아래 크기로. 

  if (r1.intersects(r2) 
    && (r1.top + r1.height <= r2.top + 10)
    && (r1.left + r1.width > r2.left + 10) // 캐릭터가 블록 좌끝보다 우측에 
    && (r1.left < r2.left + r2.width - 10) // 캐릭터가 블록 우끝보다 좌측에 
    && (this->getDy() >= 0.0f)) {
    return true;
  }
  return false;
}
bool GameCharacter::onBlock(const Sprite& block) {
  FloatRect r1 = this->getGlobalBounds();
  FloatRect r2 = block.getGlobalBounds();
  FloatRect r_tocomp(r2.left, r2.top + 15, r2.width, r2.height);
  if (r1.intersects(r_tocomp)
    && (r1.top + r1.height <= r_tocomp.top + 10)
    && (r1.left + r1.width > r_tocomp.left + 10) // 캐릭터가 블록 좌끝보다 우측에 
    && (r1.left < r_tocomp.left + r_tocomp.width - 10) // 캐릭터가 블록 우끝보다 좌측에 
    && (this->getDy() >= 0.0f)) {
    return true;
  }
  else return false;
}
/////////////////////////////////////////////////
//       캐릭터가 움직이고 있는 중인지 확인         //
/////////////////////////////////////////////////
bool GameCharacter::onMoving() {
  if (getDx() != .0f) {
    _onClimbing = false;
    return true;
  }
  else {
    return false;
  }
}
/////////////////////////////////////////////////
//             무기에 맞았는 지 확인              //
/////////////////////////////////////////////////
bool GameCharacter::onHit(const Sprite& weapon) {
  FloatRect r1 = this->getGlobalBounds();
  FloatRect r2 = weapon.getGlobalBounds();

  r1.left += 8;
  r1.top += 8;
  r1.width -= 16;
  r1.height -= 16;

  r2.left += 8;
  r2.top += 8;
  r2.width -= 16;
  r2.height -= 16;

  // weapon에 맞았을 때
  if (r1.intersects(r2)) {
    return true;
  }
  return false;
}
//////////////////////////////////////////////////
//          로프를 탈 수 있는 상태인지 확인         //
/////////////////////////////////////////////////
void GameCharacter::damagedBy(Obstacle& weapon, bool onBlock)
{
  if (this->onHit(weapon) == true)
  {
    //왼쪽으로 맞았을 때
    if (this->getDx() < 0 && onBlock)   //바닥에서 부딪힘
    {
      //this->move(-3, 0);
      this->setDx(5);
      this->setDy(-4);
    }
    else if (this->getDx() < 0 && this->getDy() != 0 && !onBlock)   //점프하면서 부딪힘
    {
      this->setDx(5);
      this->setDy(-2);
    }
    else if (this->getDx() > 0 && this->getDy() == 0 && !onBlock)
    {
      this->setDx(5);
    }
    //오른쪽에서 맞았을 때
    else if (this->getDx() >= 0 && onBlock)   //바닥에서 부딪힘
    {
      this->setDx(-5);
      this->setDy(-4);
    }
    else if (this->getDx() > 0 && this->getDy() != 0 && !onBlock)   //점프하면서 부딪힘
    {
      this->setDx(-5);
      this->setDy(-2);
    }
    else if (this->getDx() > 0 && this->getDy() == 0 && !onBlock)
    {
      this->setDx(-5);
    }
    //떨어질 때
    else if (this->getDy() > 0)
    {
      this->setDx(5);
    }
    this->decreaseHp();
  }

}
bool GameCharacter::canClimbing(const point rope[2]) {
  FloatRect r1(this->getPosition().x + 10, this->getPosition().y, 30, 70);

  int diff = rope[1].y - rope[0].y;
  int count = diff / 30;
  FloatRect r2(rope[0].x, rope[0].y, 5.0f, 30.0f * count);
  // 아바타 크기 나오면 조건 지정
  if (r1.intersects(r2)) {
    return true;
  }
  else {
    return false;
  }
}
bool GameCharacter::canClimbing(const Sprite& rope) {
  FloatRect r1 = this->getGlobalBounds();
  FloatRect r2 = rope.getGlobalBounds();
  // 아바타 크기 나오면 조건 지정
  if (r1.intersects(r2)) {
    return true;
  }
  else {
    return false;
  }
}

/////////////////////////////////////////////////
//                  캐릭터 움직임               //
////////////////////////////////////////////////
void GameCharacter::stand() {
  setDx(0.0f);
  setDy(0.0f);
}
void GameCharacter::moveLeft() {
  // 왼쪽으로 이동
  float x = this->getPosition().x;
  if (_onClimbing) {
    return;
  }
  if (x > -20.0f) {
    setDx(-1.5);
    move(getDx(), 0);
  }
  else {
    move(1, 0);
  }
  // 이미지 애니메이션
}
void GameCharacter::moveRight() {
  // 오른쪽으로 이동
  float x = this->getPosition().x;
  if (_onClimbing) {
    return;
  }
  if (x < WINDOW_WIDTH + 20) {
    setDx(1.5);
    move(getDx(), 0);
  }
  else {
    move(-1, 0);
  }
  // 이미지 애니메이션
}
void GameCharacter::jump() {
  _onClimbing = false;
  // 점프하기
  setDy(-5.5f);
  move(0, getDy());
  // 이미지 애니메이션
}
void GameCharacter::climbUp(const point p[2]) {
  FloatRect f1 = this->getGlobalBounds();
  this->setPosition(p[0].x - 25, getPosition().y);
  if (f1.top + f1.height < p[0].y + 5) {
    _onClimbing = false;
    setDy(0.0f);
    move(0.2f, -0.5f);
  }
  else {
    setDy(-1.0f);
    move(0, getDy());
    _onClimbing = true;
  }
  // 이미지 애니메이션
  if (!this->canClimbing(p)) {
    _onClimbing = false;
  }

}
void GameCharacter::climbDown(const point p[2]) {
  FloatRect f1 = this->getGlobalBounds();
  this->setPosition(p[0].x - 25, getPosition().y);
  if (f1.top + 5 > p[1].y + 30) {
    _onClimbing = false;
    setDy(0.0f);
    move(0.2f, 0.5f);
  }
  else {
    setDy(1.0f);
    move(0, getDy());
    _onClimbing = true;
  }
  // 이미지 애니메이션
  if (!this->canClimbing(p)) {
    _onClimbing = false;
  }
}

/////////////////////////////////////////////////
//             캐릭터가 받아야 할 특성            //
////////////////////////////////////////////////
void GameCharacter::gravity(bool onBlock) {
  // 중력 적용하기
  
  if (_onClimbing) {
    setDx(0.0f);
    return;
  }
  if (!onBlock)      // 바닥 450
  {
    setDy(getDy() + GRAVITY_ACC * 0.03);
    move(getDx(), getDy());
  }
  else
  {
    setDy(0.0f);
  }
}
void GameCharacter::increaseHp() {
  if (this->_hp < MAX_HP)
    this->_hp++;
}
void GameCharacter::decreaseHp() {
  if (this->_hp > 0)
    this->_hp--;
}
void GameCharacter::fall(bool onBlock, double preDy)
{
  if (10 < preDy&& onBlock)   //숫자 고치기 필요
    decreaseHp();
}

Rope::Rope() {
}
Rope::Rope(const Texture& texture) {
  setTexture(texture);
}
Rope::~Rope() {

}

// Written by 수현

Obstacle::Obstacle() {}
Obstacle::Obstacle(const Texture& b, double x, double y)
{
  setTexture(b);
  setPosition(x, y);
}
Obstacle::Obstacle(const Texture& t, IntRect r) :Sprite(t, r) {
}
Obstacle::~Obstacle() {}


Spear::Spear() {}
Spear::Spear(const Texture& spear, double x, double y)
{
  setTexture(spear);
  setPosition(x, y);
}
Spear::~Spear() {}
/*void Spear::attack(Texture& spear)
{
//setTextureRect(297, 170, 0, 0);
}*/


Monkey::Monkey() {}
Monkey::Monkey(const Texture& monkey, const Texture& banana, const IntRect& r, bool is_left) : Obstacle(monkey, r)
{
  setPosition(r.left, r.top);
  minus_hp = 3;
  this->is_left = is_left;
  this->banana.setTexture(banana);
  this->banana.setPosition(r.left + 10, r.top + 10);
  _action = false;
  _t_att = .0f;
}
Monkey::Monkey(const Texture& monkey, const Texture& banana, double x, double y, bool is_left)
{
  setTexture(monkey);
  minus_hp = 3;
  this->is_left = is_left;
  this->banana.setTexture(banana);
  this->banana.setPosition(x + 10, y + 10);
  _action = true;
  _t_att = .0f;
}

Monkey::~Monkey() {}
bool Monkey::throw_banana(float time)
{
  _t_att += time;
  if (_t_att >= 0.0f && _t_att < 2.0f) {
    if (_t_att > 1.5f) {
      banana.threw(is_left, 999.0f);
      banana.setPosition(this->getPosition().x + 10, this->getPosition().y + 10);
      return false;
    }
    _action = false;
    banana.threw(is_left, time);
    return true;
  }
  if (_t_att > 2.0f) {
    _action = true;
    _t_att = .0f;
    return false;
  }
  return false;
}

////
Banana::Banana() {
  setPosition(100, 400);
  _time = 0.0f;
}
Banana::Banana(const Texture& banana, double x, double y)
{
  setTexture(banana);
  setPosition(x, y);
  _time = 0.0f;
}
Banana::~Banana() {}
bool Banana::threw(bool is_left, float time)
{
  _time += time;
  if (is_left && _time < 2.0f) {

    this->move(-4.0f, 0);
    //std::cout << _time << std::endl;
    return true;
  }
  else if(!is_left && _time < 2.0f) {
    this->move(4.0f, 0);
    return true;
  }
  if (_time >= 2.0f) {
    _time = .0f;
    return false;
  }
}