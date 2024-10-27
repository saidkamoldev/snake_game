#include "mainwindow.h"
// Fayllarni o'qish va yozish uchun

const int FIELD_WIDTH = 10;   // Maydon kengligi
const int FIELD_HEIGHT = 20;  // Maydon balandligi
const int UNIT_SIZE = 20;     // Ilon va mevalar birligi o'lchami

void MainWindow::loadMaxScore() {
  std::ifstream inFile("max_score.txt");
  if (inFile.is_open()) {
    inFile >> maxScore;
    inFile.close();
  } else {
    maxScore = 0;  // Fayl yo'q bo'lsa, maxScore ni 0 ga o'rnatamiz
  }
}
//
void MainWindow::saveMaxScore() {
  std::ofstream outFile("max_score.txt");
  if (outFile.is_open()) {
    outFile << maxScore;
    outFile.close();
  }
}

// MainWindow konstruktori
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), timer(new QTimer(this)), appleCount(0), level(1) {

    QPixmap background("photo_bg/Amir.jpg");
    if (background.isNull() || background.width() != 947 || background.height() != 895) {
        QMessageBox::critical(nullptr, "Error", "Rasmni o'zgartira olmaysan agar buni qila olseng sanga RESPECT+");
        std::exit(EXIT_FAILURE); // Dasturdan darhol chiqish
    }
  QDir::setCurrent("photo_bg/image.png");
  loadMaxScore();  // maxScore ni fayldan yuklash
  setupGame();
  resize(FIELD_WIDTH * UNIT_SIZE, FIELD_HEIGHT * UNIT_SIZE);
  connect(timer, &QTimer::timeout, this, &MainWindow::gameLoop);
  timer->start(150);
}

MainWindow::~MainWindow() {
  // UI yo'q, shuning uchun hech narsani o'chirish kerak emas
}

// O'yinni boshlang'ich holatga qaytarish
// O'yinni boshlang'ich holatga qaytarish
void MainWindow::setupGame() {
  gameOver = false;
  isPaused = false;
  dir = RIGHT;
  x = FIELD_WIDTH / 2;
  y = FIELD_HEIGHT / 2;
  fruitX = QRandomGenerator::global()->bounded(FIELD_WIDTH);
  fruitY = QRandomGenerator::global()->bounded(FIELD_HEIGHT);

  // Score, level va appleCountni qayta tiklash
  score = 0;
  level = 0;
  appleCount = 0;

  // Ilonning boshlang'ich uzunligini 0 ga o'rnatish (faqat bosh bo'ladi)
  tailLength = 0;
  tailX[0] = x;
  tailY[0] = y;

  loadMaxScore();  // Maksimal natijani yuklash
}

// O'yin sikli
void MainWindow::gameLoop() {
  if (!gameOver) {
    moveSnake();
    checkCollisions();
    update();  // paintEvent chaqiriladi
  }
}

// Ilonning harakati
void MainWindow::moveSnake() {
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

  // Chegaradan chiqmaslik uchun
  if (x < 0) x = FIELD_WIDTH - 1;
  if (x >= FIELD_WIDTH) x = 0;
  if (y < 0) y = FIELD_HEIGHT - 1;
  if (y >= FIELD_HEIGHT) y = 0;
}

// To'qnashuvlarni tekshirish
void MainWindow::checkCollisions() {
  if (x == fruitX && y == fruitY) {
    score += 1;  // Har bir olma uchun 1 ochko qo'shish
    appleCount++;
    generateFruit();
    tailX[tailLength] = tailX[tailLength - 1];
    tailY[tailLength] = tailY[tailLength - 1];
    tailLength++;

    // Har 5 score uchun darajani yangilash
    if (score % 5 == 0) {
      level = score / 5;
    }

    // Maksimal natija yangilanayotganini tekshirish
    if (score > maxScore) {
      maxScore = score;
      saveMaxScore();  // Yangi maksimal natijani faylga saqlash
    }
  }

  for (int i = 0; i < tailLength; i++) {
    if (tailX[i] == x && tailY[i] == y) gameOver = true;
  }
}

// Olma hosil qilish
void MainWindow::generateFruit() {
  fruitX = QRandomGenerator::global()->bounded(FIELD_WIDTH);
  fruitY = QRandomGenerator::global()->bounded(FIELD_HEIGHT);
}

// Game Over xabari
void MainWindow::gameOverMessage() {
    QDialog msgBox(this);
    msgBox.setWindowTitle("Game Over");
    msgBox.setMinimumSize(700, 400);
    msgBox.setMaximumSize(700, 400);

    // Fon rasmi uchun manzil
    QPixmap background("photo_bg/Amir.jpg");

    // Rasm o'lchamini tekshirish
    if (!background.isNull() && background.width() == 947 && background.height() == 895) {
        // Agar rasm hajmi to'g'ri bo'lsa, fon rasmni o'rnatamiz
        msgBox.setStyleSheet(
            "QDialog { background-image: url(photo_bg/Amir.jpg); background-repeat: "
            "no-repeat; background-position: center; background-size: cover; }");
    } else {
        // Agar rasm noto'g'ri yoki ruxsatsiz o'zgartirilgan bo'lsa, xatolik ko'rsatish
        QMessageBox::critical(this, "Error", "Rasmni o'zgartira olmaysan agar buni qila olseng sanga RESPECT+");
        close();  // Dasturni yopish
        return;
    }

    // "Game Over" matni
    QLabel *textLabel = new QLabel(
        "<h2 style='color: red; font-size: 30px; text-shadow: 2px 2px #000;'>GAME OVER</h2>", &msgBox);
    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setGeometry(200, -10, 300, 50);

    // "Yes" va "No" tanlovlari haqida ma'lumot beruvchi matn
    QLabel *infoLabel = new QLabel(
        "<h3 style='font-size: 16px; color: #FFFFFF; font-weight: bold;'>QAYTA "
        "YUKLASH UCHUN 'YES' NI, CHIQISH UCHUN 'NO' NI TANLANG.</h3>", &msgBox);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setGeometry(100, 150, 500, 50);

    // Tugmalar uchun "neon" effekt
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    QPushButton *yesButton = new QPushButton("Yes", &msgBox);
    QPushButton *noButton = new QPushButton("No", &msgBox);

    yesButton->setStyleSheet(
        "QPushButton { color: #FFFFFF; background-color: #333; font-size: 16px; "
        "border: 2px solid #00FF00; border-radius: 10px; padding: 10px; }"
        "QPushButton:hover { background-color: #00FF00; color: #000; }");

    noButton->setStyleSheet(
        "QPushButton { color: #FFFFFF; background-color: #333; font-size: 16px; "
        "border: 2px solid #FF0000; border-radius: 10px; padding: 10px; }"
        "QPushButton:hover { background-color: #FF0000; color: #000; }");

    buttonLayout->addWidget(yesButton);
    buttonLayout->addWidget(noButton);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(textLabel);
    layout->addWidget(infoLabel);
    layout->addLayout(buttonLayout);

    msgBox.setLayout(layout);

    connect(yesButton, &QPushButton::clicked, &msgBox, &QDialog::accept);
    connect(noButton, &QPushButton::clicked, &msgBox, &QDialog::reject);

    int ret = msgBox.exec();

    if (ret == QDialog::Accepted) {
        setupGame();        // O'yinni qayta boshlash
        timer->start(150);  // Taymerni qayta ishga tushirish
    } else if (ret == QDialog::Rejected) {
        close();  // Dasturdan chiqish
    }
}


void MainWindow::drawNeonGameBorder(QPainter &painter, int x, int y, int width,
                                    int height) {
  drawNeonRect(painter, x, y, width, height, QColor(138, 43, 226),
               QColor(75, 0, 130));  // Neon effekt
}

void MainWindow::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);

  QPainter painter(this);
  painter.setBrush(Qt::black);
  painter.drawRect(0, 0, this->width(), this->height());
  // Shifrlangan yoki maxsus rasmni yuklash
  QPixmap background("photo_bg/origin_Demetrif.png");  // Asl rasm joylashuvi
  if (!background.isNull() && background.width() == 2112 &&
      background.height() == 1188) {  // Original rasm hajmini tekshiruv
    painter.drawPixmap(this->rect(),
                       background);  // To'liq oyna o'lchamida chizadi
  } else {
    // Agar rasm noto'g'ri yoki ruxsatsiz o'zgartirilgan bo'lsa, dasturni
    // to'xtatish yoki xabar ko'rsatish
    QMessageBox::critical(this, "Error", "Invalid background image!");
    close();  // Dasturni yopadi yoki xato funksiyasiga o'tadi
    return;
  }
  int padding = 30;     // To'rtta tarafdan padding
  int marginLeft = 20;  // Margin-left qiymati

  // Ilon o'yinini markazlash uchun o'lchamlar
  int gameAreaWidth = FIELD_WIDTH * UNIT_SIZE;
  int gameAreaHeight = FIELD_HEIGHT * UNIT_SIZE;

  // O'ng tomondagi konteynerlar kengligi va balandligi
  int containersWidth = 220;
  // int containersHeight = gameAreaHeight;  // Katta konteyner uchun balandlik

  // Umumiy kenglik va balandlikni hisoblash (o'yin maydoni + konteynerlar)
  int totalWidth =
      gameAreaWidth + containersWidth + 60;  // 60 - oradagi bo'sh joy
  int totalHeight = gameAreaHeight;

  // Barcha elementlarni ekranning markaziga joylashtirish
  int centerX = (this->width() - totalWidth) / 2;
  int centerY = (this->height() - totalHeight) / 2;

  // O'yin maydonini markazlashtirish
  int offsetX = centerX;
  int offsetY = centerY;

  // Chegaralarni o'rnatish (padding har 4 tarafdan)
  offsetX += padding;
  offsetY += padding;

  // Devorlar
  painter.setPen(QPen(Qt::white, 5));
  painter.drawRect(offsetX - padding, offsetY - padding,
                   gameAreaWidth + padding * 2, gameAreaHeight + padding * 2);
  // Tashqi devorlarni neon effektli qilish uchun
  drawNeonRect(painter, offsetX - padding, offsetY - padding,
               gameAreaWidth + padding * 2, gameAreaHeight + padding * 2,
               QColor(138, 43, 226),
               QColor(75, 0, 130));  // Neon effektli devorlar

  // Har bir katakni oq chiziq bilan o'rab olish
  painter.setPen(QPen(QColor(255, 255, 255, 76),
                      1));  // Oq rang (white) bilan 30% shaffoflik (alpha 76)
  for (int i = 0; i < FIELD_WIDTH; i++) {
    for (int j = 0; j < FIELD_HEIGHT; j++) {
      painter.drawRect(offsetX + i * UNIT_SIZE, offsetY + j * UNIT_SIZE,
                       UNIT_SIZE, UNIT_SIZE);
    }
  }

  // O'ng tomondagi konteynerlar uchun joylashuv
  int textOffsetX =
      offsetX + gameAreaWidth + 40 + marginLeft;  // margin-left qo'shildi

  // Max Score konteyneri
  int maxScoreWidth = 220;
  int maxScoreHeight = 60;
  drawMaxScore(painter, textOffsetX, centerY, maxScoreWidth, maxScoreHeight);

  // Score konteyneri uchun o'lchamlar
  int scoreWidth = 220;
  int scoreHeight = 60;
  drawScore(painter, textOffsetX, centerY + maxScoreHeight + 25, scoreWidth,
            scoreHeight);

  // Apple Count konteyneri uchun o'lchamlar
  int appleCountWidth = 220;
  int appleCountHeight = 60;
  drawAppleCount(painter, textOffsetX,
                 centerY + maxScoreHeight + scoreHeight + 45, appleCountWidth,
                 appleCountHeight);

  // Controls konteyneri uchun o'lchamlar
  int controlsWidth = 220;
  int controlsHeight = 205;
  drawControls(painter, textOffsetX,
               centerY + maxScoreHeight + scoreHeight + appleCountHeight + 70,
               controlsWidth, controlsHeight);

  // Ilonning boshini chizish
  QPixmap headPixmap("photo_bg/snake.svg");
  QPixmap bodyPixmap("photo_bg/body.svg");
  QPixmap applePixmap("photo_bg/olma_1000.png");

  if (!headPixmap.isNull()) {
    painter.drawPixmap(offsetX + x * UNIT_SIZE, offsetY + y * UNIT_SIZE,
                       UNIT_SIZE, UNIT_SIZE, headPixmap);
  } else {
    painter.setBrush(Qt::blue);
    painter.drawRect(offsetX + x * UNIT_SIZE, offsetY + y * UNIT_SIZE,
                     UNIT_SIZE, UNIT_SIZE);
  }

  // Ilonning tanasi
  if (!bodyPixmap.isNull()) {
    for (int i = 0; i < tailLength; i++) {
      painter.drawPixmap(offsetX + tailX[i] * UNIT_SIZE,
                         offsetY + tailY[i] * UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,
                         bodyPixmap);
    }
  } else {
    painter.setBrush(Qt::green);
    for (int i = 0; i < tailLength; i++) {
      painter.drawRect(offsetX + tailX[i] * UNIT_SIZE,
                       offsetY + tailY[i] * UNIT_SIZE, UNIT_SIZE, UNIT_SIZE);
    }
  }

  // Olmani chizish
  int appleSize = UNIT_SIZE * 1.3;
  if (!applePixmap.isNull()) {
    painter.drawPixmap(offsetX + fruitX * UNIT_SIZE,
                       offsetY + fruitY * UNIT_SIZE, appleSize, appleSize,
                       applePixmap);
  } else {
    painter.setBrush(Qt::red);
    painter.drawEllipse(offsetX + fruitX * UNIT_SIZE,
                        offsetY + fruitY * UNIT_SIZE, appleSize, appleSize);
  }

  // O'yin tugaganda
  if (gameOver) {
    gameOverMessage();
  }

  // O'yin pauza holati
  if (isPaused) {
    painter.setPen(Qt::yellow);
    painter.drawText(this->width() / 2 - 50, this->height() / 2, "Paused");
  }
}

// Kalitbosim voqealari
void MainWindow::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_W && dir != DOWN && !isPaused && !gameOver)
    dir = UP;
  if (event->key() == Qt::Key_S && dir != UP && !isPaused && !gameOver)
    dir = DOWN;
  if (event->key() == Qt::Key_A && dir != RIGHT && !isPaused && !gameOver)
    dir = LEFT;
  if (event->key() == Qt::Key_D && dir != LEFT && !isPaused && !gameOver)
    dir = RIGHT;

  // Pauza
  if (event->key() == Qt::Key_P) {
    isPaused = !isPaused;
    if (isPaused)
      timer->stop();
    else
      timer->start(150);
  }

  // O'yinni qayta boshlash
  if (event->key() == Qt::Key_R && gameOver) {
    setupGame();
    timer->start(150);
  }

  // O'yinni yopish
  if (event->key() == Qt::Key_Q) {
    close();  // Dasturni to'xtatish yoki yopish
  }
}

void MainWindow::drawNeonRect(QPainter &painter, int x, int y, int width,
                              int height, QColor color1, QColor color2) {
  // Chuqur porlash effekti uchun ko'proq yarim shaffof chiziqlar chizamiz
  for (int i = 0; i < 6; i++) {
    QColor glowColor = color2;
    glowColor.setAlpha(60 - i * 10);  // Yarim shaffof qatlamlar
    QPen glowPen(glowColor);
    glowPen.setWidth(9 + i * 2);  // Har bir qatlam tashqariga kengayib boradi
    painter.setPen(glowPen);
    painter.drawRect(x - i, y - i, width + 2 * i, height + 2 * i);
  }

  // Ichki neon chiziq (yorqin qismi)
  QPen innerNeonPen(color1);
  innerNeonPen.setWidth(3);
  painter.setPen(innerNeonPen);
  painter.drawRect(x, y, width, height);

  // Shadow effekti (qo'shimcha porlash)
  for (int i = 0; i < 3; i++) {
    QColor shadowColor = color1.darker(150);  // Shadow uchun quyuqroq rang
    shadowColor.setAlpha(100 - i * 50);       // Shadowning yarim shaffofligi
    QPen shadowPen(shadowColor);
    shadowPen.setWidth(12 + i * 4);  // Shadowni yanada kengaytiramiz
    painter.setPen(shadowPen);
    painter.drawRect(x - i, y - i, width + 2 * i, height + 2 * i);
  }
}

void MainWindow::drawMaxScore(QPainter &painter, int offsetX, int offsetY,
                              int width, int height) {
  drawNeonRect(painter, offsetX, offsetY, width, height, QColor(138, 43, 226),
               QColor(75, 0, 130));  // Siyohrang neon ranglar

  // Yashil neon rangli matn
  painter.setPen(QColor(57, 255, 20));  // Yashil neon rang
  painter.drawText(offsetX + 10, offsetY + 30,
                   QString("Max Score: %1").arg(maxScore));
}

void MainWindow::drawScore(QPainter &painter, int offsetX, int offsetY,
                           int width, int height) {
  drawNeonRect(painter, offsetX, offsetY, width, height, QColor(138, 43, 226),
               QColor(75, 0, 130));  // Siyohrang neon ranglar

  // Yashil neon rangli matn
  painter.setPen(QColor(57, 255, 20));  // Yashil neon rang
  painter.drawText(offsetX + 10, offsetY + 30,
                   QString("Score: %1  Level: %2").arg(score).arg(level));
}

void MainWindow::drawAppleCount(QPainter &painter, int offsetX, int offsetY,
                                int width, int height) {
  drawNeonRect(painter, offsetX, offsetY, width, height, QColor(138, 43, 226),
               QColor(75, 0, 130));  // Siyohrang neon ranglar

  // Yashil neon rangli matn
  painter.setPen(QColor(57, 255, 20));  // Yashil neon rang
  painter.drawText(offsetX + 10, offsetY + 30,
                   QString("Yeyilgan Olmalar Soni: %1").arg(appleCount));
}

void MainWindow::drawControls(QPainter &painter, int offsetX, int offsetY,
                              int width, int height) {
  drawNeonRect(painter, offsetX, offsetY, width, height, QColor(138, 43, 226),
               QColor(75, 0, 130));  // Siyohrang neon ranglar

  // Yashil neon rangli controls matnlar
  painter.setPen(QColor(57, 255, 20));  // Yashil neon rang
  painter.drawText(offsetX + 10, offsetY + 30,
                   "W - Yuqoriga");  // Oldingi 60 o'rniga 40
  painter.drawText(offsetX + 10, offsetY + 65,
                   "S - Pastga");  // Oldingi 85 o'rniga 65
  painter.drawText(offsetX + 10, offsetY + 90,
                   "A - Chapga");  // Oldingi 110 o'rniga 90
  painter.drawText(offsetX + 10, offsetY + 115,
                   "D - O'ngga");  // Oldingi 135 o'rniga 115
  painter.drawText(offsetX + 10, offsetY + 140,
                   "R - Qayta boshlash");  // Oldingi 160 o'rniga 140
  painter.drawText(offsetX + 10, offsetY + 165,
                   "P - Pauza");  // Oldingi 185 o'rniga 165
  painter.drawText(offsetX + 10, offsetY + 190,
                   "Q - Chiqish");  // Oldingi 210 o'rniga 190
}
