#include <QApplication>
#include <QDir>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  // Ishchi katalogni o'rnatish
  QDir::setCurrent("photo_bg/image.png");

  MainWindow w;
  w.show();
  return a.exec();
}
