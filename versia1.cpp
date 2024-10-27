#include <ncurses.h>  // ncurses kutubxonasi
#include <unistd.h>

#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

// O'zgaruvchilar e'lon qilish
bool gameOver;
bool invalidCoord;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int tailLength;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir;

// Ekranni tozalash
void ClearScreen() {
  clear();  // ncurses funksiyasi orqali ekranni tozalash
  refresh();
}

// Boshlang'ich sozlashlar
void Setup() {
  gameOver = false;
  dir = STOP;
  srand(time(0));
  x = width / 2;
  y = height / 2;
  fruitX = rand() % width;
  fruitY = rand() % height;
  score = 0;

  initscr();    // ncurses ekranni ishga tushirish
  clear();      // Tozalash
  noecho();     // Kiritmani ko'rsatmaslik
  cbreak();     // Kiritmani to'g'ridan-to'g'ri qabul qilish
  curs_set(0);  // Kursorni yashirish
}

// Chizish funksiyasi
void Draw() {
  ClearScreen();

  // Yuqori chegara
  for (int i = 0; i < width + 2; i++) printw("-");
  printw("\n");

  for (int i = 0; i < height; i++) {
    for (int k = 0; k < width; k++) {
      // Chap chegara
      if (k == 0) printw("|");

      // Ilonning boshi
      if (i == y && k == x) printw("@");
      // Meva
      else if (i == fruitY && k == fruitX)
        printw("*");
      else {
        bool printTail = false;
        for (int j = 0; j < tailLength; j++) {
          if (tailX[j] == k && tailY[j] == i) {
            printw("o");
            printTail = true;
          }
        }
        if (!printTail) printw(" ");
      }

      // O'ng chegara
      if (k == width - 1) printw("|");
    }
    printw("\n");
  }

  // Quyidagi chegara
  for (int i = 0; i < width + 2; i++) printw("-");
  printw("\n");

  // O'yinchi balini ko'rsatish
  printw("Score: %d\n", score);
  refresh();
}

// Kiritmalarni olish
void Input() {
  keypad(stdscr, TRUE);  // Klaviatura kiritmalarini yoqish
  halfdelay(1);          // Non-blocking input

  int c = getch();
  switch (c) {
    case 'w':
      if (dir != DOWN) dir = UP;
      break;
    case 'a':
      if (dir != RIGHT) dir = LEFT;
      break;
    case 's':
      if (dir != UP) dir = DOWN;
      break;
    case 'd':
      if (dir != LEFT) dir = RIGHT;
      break;
    case 'k':
      gameOver = true;
      break;
  }
}

// O'yin logikasi
void Logic() {
  int prevX = tailX[0];
  int prevY = tailY[0];
  int prevX2, prevY2;
  tailX[0] = x;
  tailY[0] = y;

  for (int i = 1; i < tailLength; i++) {
    prevX2 = tailX[i];
    prevY2 = tailY[i];
    tailX[i] = prevX;
    tailY[i] = prevY;
    prevX = prevX2;
    prevY = prevY2;
  }

  switch (dir) {
    case LEFT:
      x--;
      break;
    case RIGHT:
      x++;
      break;
    case UP:
      y--;
      break;
    case DOWN:
      y++;
      break;
  }

  // Ilonning meva yeyishi
  if (x == fruitX && y == fruitY) {
    score += 1;
    fruitX = rand() % width;
    fruitY = rand() % height;
    tailLength++;
  }

  // Chegaralardan o'tib ketish
  if (y >= height)
    gameOver = true;
  else if (y < 0)
    y = gameOver = true;
  if (x >= width)
    gameOver = true;
  else if (x < 0)
    x = gameOver = true;

  // Ilonning o'zi bilan to'qnashuvi
  for (int i = 0; i < tailLength; i++)
    if (tailX[i] == x && tailY[i] == y) gameOver = true;
}

int main() {
  Setup();

  while (!gameOver) {
    Draw();
    usleep(100000);  // Harakatni sekinlashtirish
    Input();
    Logic();
  }

  endwin();  // ncurses ekranini yopish
  return 0;
}

// g++ main.cpp -o snake -lncurses
