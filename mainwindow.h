#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDir>
#include <QFile>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QPushButton>
#include <QRandomGenerator>
#include <QTextStream>
#include <QTime>
#include <QTimer>
#include <QVBoxLayout>
#include <fstream>

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 protected:
  void paintEvent(QPaintEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;

 private slots:
  void gameLoop();

 private:
  void setupGame();
  void moveSnake();
  void checkCollisions();
  void loadMaxScore();  // Max score-ni fayldan yuklash
  void saveMaxScore();
  void generateFruit();
  void gameOverMessage();
  void drawMaxScore(QPainter &painter, int offsetX, int offsetY, int width,
                    int height);
  void drawScore(QPainter &painter, int offsetX, int offsetY, int width,
                 int height);
  void drawAppleCount(QPainter &painter, int offsetX, int offsetY, int width,
                      int height);
  void drawNeonRect(QPainter &painter, int x, int y, int width, int height,
                    QColor color1, QColor color2);
  void drawNeonGameBorder(QPainter &painter, int x, int y, int width,
                          int height);
  void drawControls(QPainter &painter, int offsetX, int offsetY, int width,
                    int height);

  QTimer *timer;
  int maxScore;
  int score;
  int appleCount;
  int level;  // Yangi o'zgaruvchi - daraja
  int x, y, fruitX, fruitY;
  int tailX[100], tailY[100], tailLength;
  bool gameOver, isPaused;
  enum Direction { UP, DOWN, LEFT, RIGHT } dir;
};

#endif  // MAINWINDOW_H
