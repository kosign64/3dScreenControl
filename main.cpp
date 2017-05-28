#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // Создание объекта необходимого для обработки событий
    // в графическом интерфейсе
    QApplication a(argc, argv);
    // Создание объекта нашего класса, описанного
    // в mainwindow.h и mainwindow.cpp
    MainWindow w;
    // Вызов метода для показа окна
    w.show();

    // Запуск "бесконечного" цикла обработки
    // событий
    return a.exec();
}
