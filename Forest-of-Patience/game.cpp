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
//       ĳ���Ͱ� ����� ���� ��ġ�� �ִ� �� Ȯ��    //
/////////////////////////////////////////////////
bool GameCharacter::onBlock(const point& block) {
  FloatRect r1 = this->getGlobalBounds();
  FloatRect r2(block.x, block.y + 15, 32.0f, 46.0f); // 10��ŭ �Ʒ� ũ���. 

  if (r1.intersects(r2) 
    && (r1.top + r1.height <= r2.top + 10)
    && (r1.left + r1.width > r2.left + 10) // ĳ���Ͱ� ��� �³����� ������ 
    && (r1.left < r2.left + r2.width - 10) // ĳ���Ͱ� ��� �쳡���� ������ 
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
    && (r1.left + r1.width > r_tocomp.left + 10) // ĳ���Ͱ� ��� �³����� ������ 
    && (r1.left < r_tocomp.left + r_tocomp.width - 10) // ĳ���Ͱ� ��� �쳡���� ������ 
    && (this->getDy() >= 0.0f)) {
    return true;
  }
  else return false;
}
/////////////////////////////////////////////////
//       ĳ���Ͱ� �����̰� �ִ� ������ Ȯ��         //
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
//             ���⿡ �¾Ҵ� �� Ȯ��              //
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

  // weapon�� �¾��� ��
  if (r1.intersects(r2)) {
    return true;
  }
  return false;
}
//////////////////////////////////////////////////
//          ������ Ż �� �ִ� �������� Ȯ��         //
/////////////////////////////////////////////////
void GameCharacter::damagedBy(Obstacle& weapon, bool onBlock)
{
  if (this->onHit(weapon) == true)
  {
    //�������� �¾��� ��
    if (this->getDx() < 0 && onBlock)   //�ٴڿ��� �ε���
    {
      //this->move(-3, 0);
      this->setDx(5);
      this->setDy(-4);
    }
    else if (this->getDx() < 0 && this->getDy() != 0 && !onBlock)   //�����ϸ鼭 �ε���
    {
      this->setDx(5);
      this->setDy(-2);
    }
    else if (this->getDx() > 0 && this->getDy() == 0 && !onBlock)
    {
      this->setDx(5);
    }
    //�����ʿ��� �¾��� ��
    else if (this->getDx() >= 0 && onBlock)   //�ٴڿ��� �ε���
    {
      this->setDx(-5);
      this->setDy(-4);
    }
    else if (this->getDx() > 0 && this->getDy() != 0 && !onBlock)   //�����ϸ鼭 �ε���
    {
      this->setDx(-5);
      this->setDy(-2);
    }
    else if (this->getDx() > 0 && this->getDy() == 0 && !onBlock)
    {
      this->setDx(-5);
    }
    //������ ��
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
  // �ƹ�Ÿ ũ�� ������ ���� ����
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
  // �ƹ�Ÿ ũ�� ������ ���� ����
  if (r1.intersects(r2)) {
    return true;
  }
  else {
    return false;
  }
}

/////////////////////////////////////////////////
//                  ĳ���� ������               //
////////////////////////////////////////////////
void GameCharacter::stand() {
  setDx(0.0f);
  setDy(0.0f);
}
void GameCharacter::moveLeft() {
  // �������� �̵�
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
  // �̹��� �ִϸ��̼�
}
void GameCharacter::moveRight() {
  // ���������� �̵�
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
  // �̹��� �ִϸ��̼�
}
void GameCharacter::jump() {
  _onClimbing = false;
  // �����ϱ�
  setDy(-5.5f);
  move(0, getDy());
  // �̹��� �ִϸ��̼�
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
  // �̹��� �ִϸ��̼�
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
  // �̹��� �ִϸ��̼�
  if (!this->canClimbing(p)) {
    _onClimbing = false;
  }
}

/////////////////////////////////////////////////
//             ĳ���Ͱ� �޾ƾ� �� Ư��            //
////////////////////////////////////////////////
void GameCharacter::gravity(bool onBlock) {
  // �߷� �����ϱ�
  
  if (_onClimbing) {
    setDx(0.0f);
    return;
  }
  if (!onBlock)      // �ٴ� 450
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
  if (10 < preDy&& onBlock)   //���� ��ġ�� �ʿ�
    decreaseHp();
}

Rope::Rope() {
}
Rope::Rope(const Texture& texture) {
  setTexture(texture);
}
Rope::~Rope() {

}

// Written by ����

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