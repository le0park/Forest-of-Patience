#include "game.h"
#include "Animation.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
void drawRope(RenderWindow&, Sprite&, const point p[][2], int);
void loadTexture(vector<vector<Texture>>&);
bool ReadMap(point[], point[][2]);

enum { UP, DOWN, LEFT, RIGHT, JUMP, CLIMB, MONKEY, BANANA, SPEAR } action;

int main() {
  RenderWindow app(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Forest of Patience");
  sf::View view2(Vector2f(WINDOW_WIDTH / 2, 3 * WINDOW_HEIGHT / 2),
    Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));

  app.setFramerateLimit(60); // fix 60 frame per sec

  // texture를 담아둘 image를 character, obstacle의 행동별로 담음
  vector<vector<Texture>> t_char(9, vector<Texture>(2));
  loadTexture(t_char);
  ///////////////////////////////////////////////////////////
  //                  캐릭터 애니메이션 초기화                 //
  ///////////////////////////////////////////////////////////

  vector<Animation> anim_char({
    // character action
    Animation(&t_char[UP][0], sf::Vector2u(1, 1), 0.2f),
    Animation(&t_char[DOWN][0], sf::Vector2u(1, 1), 0.2f),  //  크기 다름 70*45
    Animation(&t_char[LEFT][0], sf::Vector2u(3, 1), 0.2f),
    Animation(&t_char[RIGHT][0], sf::Vector2u(4, 1), 0.2f),
    Animation(&t_char[JUMP][0], sf::Vector2u(1, 1), 0.2f),
    Animation(&t_char[CLIMB][0], sf::Vector2u(2, 1), 0.2f),
    // obstacle action
    Animation(&t_char[MONKEY][0], sf::Vector2u(10, 1), 0.2f),
    Animation(&t_char[BANANA][0], sf::Vector2u(1, 1), 0.2f),
    Animation(&t_char[SPEAR][0], sf::Vector2u(3, 1), 0.2f),
  });
  ///////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////
  //                    Spear 애니메이션                     //
  ///////////////////////////////////////////////////////////
  RectangleShape attack(Vector2f(68.0f, 39.0f));
  attack.setPosition(100.0f, 500.0f);
  Texture t_spear;
  t_spear.loadFromFile("images/spear.png");
  attack.setTexture(&t_spear);

  Animation animation(&t_spear, sf::Vector2u(4, 1), 0.4f);

  float deltaTime = 0.0f;
  Clock clock;
  ///////////////////////////////////////////////////////////

  Texture tBack, tBlock, tFloor, tRope;

  //tBack.loadFromFile("images/Elin_background.png"); // 배경화면
  tBack.loadFromFile("images/background.png"); // 배경화면
  tBlock.loadFromFile("images/block.png"); // 블록
  tFloor.loadFromFile("images/floor.png");  // 밑바닥
  tRope.loadFromFile("images/rope.png"); // 로프
  Sprite sBackground(tBack), sPlat(tBlock), sFloor(tFloor), sRope(tRope);
  GameCharacter gPers(t_char[UP][1]);
  Monkey monkey(t_char[MONKEY][0], t_char[BANANA][0],
    IntRect(Vector2i(1000, 1207), Vector2i(56, 54)), true);

  ///////////////////////////////////////////////////////////
  //                게임 내 이벤트 초기화                     //
  ///////////////////////////////////////////////////////////
  bool onBlock = false;
  bool onMoving = false;
  double pre = 0;   // 바닥에 떨어지기 직전의 dy 값 저장
  point plat[20];   // block의 xy 값 저장
  point spear[20];  // spear의 xy 값 저장
  point rope[20][2]; // rope가 시작하는 xy, 끝나는 xy값 저장 (10 * 30)

  if (!ReadMap(plat, rope)) {
    return -1;
  }

  int x = 100, y = 100, h = 200;
  float dx = 0, dy = 0;
  int count = 0;
  sFloor.setPosition(0, 600.0f + WINDOW_HEIGHT);
  while (app.isOpen())
  {
    ///////////////////////////////////////////////////////////
    //          animation 에 필요한 deltaTime 계산             //
    ///////////////////////////////////////////////////////////
    deltaTime = clock.restart().asSeconds();
    ///////////////////////////////////////////////////////////
    Event e;
    while (app.pollEvent(e))
    {
      if (e.type == Event::Closed)
        app.close();
    }
    ///////////////////////////////////////////////////////////
    //                 키보드 버튼 이벤트 처리                  //
    ///////////////////////////////////////////////////////////
    if (Keyboard::isKeyPressed(Keyboard::Right) && gPers.getDy() <= 0) {
      // 방향키 오른쪽
      gPers.moveRight();
      // set animation sheet
      // set characters frame in sheet
      // update frame
      gPers.setTexture(t_char[RIGHT][0]);
      gPers.setTextureRect(anim_char[RIGHT].uvRect);
      anim_char[RIGHT].Update(0, deltaTime);
    }
    if (Keyboard::isKeyPressed(Keyboard::Left) && gPers.getDy() <= 0) {
      // 방향키 왼쪽
      gPers.moveLeft();

      gPers.setTexture(t_char[LEFT][0]);
      gPers.setTextureRect(anim_char[LEFT].uvRect);
      anim_char[LEFT].Update(0, deltaTime);
    }

    if (Keyboard::isKeyPressed(Keyboard::Right)
      && Keyboard::isKeyPressed(Keyboard::LAlt)
      && gPers.onClimbing()) {
      // 줄을 타고 있을 때 방향키 오른쪽 점프
      gPers.moveRight();
      gPers.jump();

      gPers.setTexture(t_char[JUMP][1], true);
    }
    if (Keyboard::isKeyPressed(Keyboard::Left)
      && Keyboard::isKeyPressed(Keyboard::LAlt)
      && gPers.onClimbing()) {
      // 줄을 타고 있을 때 방향키 왼쪽 점프
      gPers.moveLeft();
      gPers.jump();

      gPers.setTexture(t_char[JUMP][0], true);
    }
    if (Keyboard::isKeyPressed(Keyboard::Down)) {
      // 방향키 아래쪽
      gPers.setTexture(t_char[DOWN][0], true);
    }
    if (Keyboard::isKeyPressed(Keyboard::LAlt) && onBlock) {
      // 왼쪽 Alt 키 (점프)
      gPers.jump();
      if (gPers.getDx() >= 0) {
        gPers.setTexture(t_char[JUMP][1], true);
      }
      else {
        gPers.setTexture(t_char[JUMP][0], true);
      }
    }
    else {
      //gPers.setTexture(character[UP][0], true);
    }
    ///////////////////////////////////////////////////////////


    ///////////////////////////////////////////////////////////
    //                     바닥 판정 테스트                    //
    ///////////////////////////////////////////////////////////
    onBlock = gPers.onBlock(sFloor);
    onMoving = gPers.onMoving();
    pre = gPers.getDy();
    for (int i = 0; i < 10; i++) {
      if (gPers.onBlock(plat[i]) || onBlock) {
        onBlock = true;
        gPers.stand();
        gPers.fall(onBlock, pre);

        ///////////////////////////////////////////////////////////
        //                    스크롤 판정 테스트                    //
        ///////////////////////////////////////////////////////////
        if (gPers.getPosition().y < (view2.getCenter().y - WINDOW_HEIGHT / 4))// && gPers.getPosition().y >0)
        {
          view2.move(0, -WINDOW_HEIGHT / 2);
          //break;
        }
        if (gPers.getPosition().y > (view2.getCenter().y + WINDOW_HEIGHT / 4)
          && view2.getCenter().y < 707 * 2 - 3 * WINDOW_HEIGHT / 4) //707 *2 -> 나중에 전체 맵 길이 정해지면 변수로 바꿈
        {
          //cout << "up" << endl;
          std::cout << "3)" << gPers.getPosition().y << endl;
          view2.move(0, WINDOW_HEIGHT / 2);
        }
        ///////////////////////////////////////////////////////////

        break;
      }
      if (i == 9 && !onBlock) {
        //std::cout << "not standing on blocks and floor" << std::endl;
        onBlock = false;
        //view2.move(0, 2WINDOW_HEIGHT /2);

        ///////////////////////////////////////////////////////////
        //                    스크롤 판정 테스트                    //
        ///////////////////////////////////////////////////////////
        if (gPers.getPosition().y < (WINDOW_HEIGHT / 4) && gPers.getPosition().y >0)
        {
          view2.move(0, -30);
        }
        ///////////////////////////////////////////////////////////

        break;
      }
    }
    ///////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////
    //                     로프 판정 테스트                    //
    ///////////////////////////////////////////////////////////
    for (int i = 0; i < 10; i++) {
      if (gPers.canClimbing(rope[i])) {
        /*std::cout << "onclimbing" << std::endl;*/
        /*std::cout << gPers.getDx() << std::endl;
        std::cout << gPers.getDy() << std::endl;*/
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
          gPers.climbUp(rope[i]);

          gPers.setTexture(t_char[CLIMB][0]);
          gPers.setTextureRect(anim_char[CLIMB].uvRect);
          anim_char[CLIMB].Update(0, deltaTime);
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) {
          gPers.climbDown(rope[i]);

          gPers.setTexture(t_char[CLIMB][0]);
          gPers.setTextureRect(anim_char[CLIMB].uvRect);
          anim_char[CLIMB].Update(0, deltaTime);
        }
      }
      else {
      }
      if (i == 9 && !onBlock) {

      }
    }
    ///////////////////////////////////////////////////////////
    animation.Update(0, deltaTime);
    attack.setTextureRect(animation.uvRect);
    app.draw(attack);
    ///////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////
    //                     원숭이 맞는 거 판정                  //
    ///////////////////////////////////////////////////////////
    if ((gPers.onHit(monkey) || gPers.onHit(monkey.banana))
      && count == 0)
    {
      count = 60;
      gPers.damagedBy(monkey, onBlock);
      gPers.damagedBy(monkey.banana, onBlock);
    }
    if (count != 0)
      count--;
    ///////////////////////////////////////////////////////////
    gPers.gravity(onBlock);

    // Sprite 그리기
    app.draw(sBackground);
    app.draw(gPers);
    app.draw(sFloor);
    ///////////////////////////////////////////////////////////
    //                     원숭이 화면에 그리기                 //
    ///////////////////////////////////////////////////////////
    monkey.throw_banana(deltaTime);
    anim_char[MONKEY].Update(0, deltaTime);
    monkey.setTextureRect(anim_char[MONKEY].uvRect);
    app.draw(monkey.banana);
    app.draw(monkey);
    ///////////////////////////////////////////////////////////
    std::cout << gPers.getHp() << endl;

    for (int i = 0; i < 10; i++)
    {
      sPlat.setPosition(plat[i].x, plat[i].y);
      app.draw(sPlat);
    }
    drawRope(app, sRope, rope, 10);
    app.display();
    app.setView(view2);
  }

  return 0;
}

///////////////////////////////////////////////////////////
//                       로프 그리기                       //
///////////////////////////////////////////////////////////
void drawRope(RenderWindow& window, Sprite& s, const point p[][2], int size) {
  // 기본 높이를 30으로 두고 30의 배수 만큼 start, end point 차이를 둠
  int diff = 0;
  for (int i = 0; i < size; i++) {
    diff = p[i][1].y - p[i][0].y;

    int count = diff / 30;
    for (int j = 0; j < count; j++) {
      s.setPosition(p[i][0].x, p[i][0].y + j * 30);
      window.draw(s);
    }
  }
}
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
//                 게임 내 텍스쳐 그리기                    //
///////////////////////////////////////////////////////////
void loadTexture(vector<vector<Texture>>& v) {
  // single image
  v[UP][0].loadFromFile("images/char/stand_right.png");
  v[UP][1].loadFromFile("images/char/stand_right.png");
  v[JUMP][0].loadFromFile("images/char/jump_left.png");
  v[JUMP][1].loadFromFile("images/char/jump_right.png");
  v[DOWN][0].loadFromFile("images/char/lie_left.png");
  v[DOWN][1].loadFromFile("images/char/lie_right.png");
  v[BANANA][0].loadFromFile("images/banana.png");
  v[SPEAR][0].loadFromFile("images/spear.png");

  // animation sheet
  v[LEFT][0].loadFromFile("images/char/anim_left.png");
  v[RIGHT][0].loadFromFile("images/char/anim_right.png");
  v[CLIMB][0].loadFromFile("images/char/anim_climb.png");
  v[MONKEY][0].loadFromFile("images/monkey/anim_lupin.png");
}
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
//                   맵 파일 입출력 모듈                    //
///////////////////////////////////////////////////////////
bool ReadMap(point plat[], point rope[][2]) {
  ifstream fin("plat_map.txt");
  string line;
  string _x;
  string _y;
  string i_th;
  int i = 0;
  if (!fin.is_open()) {
    std::cout << "Cannot open file " << "\"plat_map.txt\"." << std::endl;
    return false;
  }
  while (!fin.eof())
  {
    getline(fin, line);
    if (line == "")
      break;
    i_th = (line.substr(0, line.find_first_of(" ")));
    line = line.substr(i_th.size() + 1, line.size());
    _x = (line.substr(0, line.find_first_of(" ")));
    _y = line.substr(_x.size() + 1, line.size());
    plat[i].x = atoi(_x.c_str());
    plat[i].y = atoi(_y.c_str()) + WINDOW_HEIGHT;
    i++;
  }
  for (int i = 0; i < 19; i++)
  {
    // 밧줄 랜덤화
    rope[i][0].x = plat[i].x;
    rope[i][0].y = plat[i].y;
    rope[i][1].x = plat[i].x;
    rope[i][1].y = plat[i].y + 90;
  }
  return true;
}
///////////////////////////////////////////////////////////