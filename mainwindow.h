#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <NiTE.h>
#include "comport.h"

// Объявление используемых классов из
// библиотеки Qt
class QVBoxLayout;
class QLabel;
// Использование пространств имён nite и
// openni по умолчнию, чтобы не писать
// nite:: и openni::
using namespace nite;
using namespace openni;

// Объявление класса MainWindow унаследованного
// от класса QMainWindow из библиотеки Qt
class MainWindow : public QMainWindow
{
    // Макрос необходимый для работы сигналов
    // и слотов в библиотеке Qt. (Является
    // расширением языка С++, не входящим в
    // стандарт, но в данной программе сигналы
    // и слоты не используются)
    Q_OBJECT

    // В открытой секции только конструктор и
    // деструктор класса
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // Переопределение методов из класса
    // QMainWindow вызываемых при срабатывании
    // таймера и нажатии на клавишу на клавиатуре
protected:
    void timerEvent(QTimerEvent*);
    void keyPressEvent(QKeyEvent *ev);

private:
    // Объявление структуры для хранения
    // трёхмерных координат точки на 3д экране
    struct Point3D
    {
        uint8_t angle;
        int8_t  x;
        uint8_t y;
    };
    // Объявление структуры для хранения
    // двухмерных координат точки на карте
    // глубины
    struct Point2D
    {
        float x;
        float y;
    };

    UserTracker userTracker;
    UserTrackerFrameRef userTrackerFrame;
    ComPort port;

    // Объект для хранения изображения
    // карты глубины
    QImage depthImage;

    // Графический интерфейс
    // Главный виджет
    QWidget *mainWidget;
    // Компановка главного виджета
    QVBoxLayout *mainLayout;
    // Виджет для отображения изображения
    QLabel *imageLabel;

    // Ваша функция масштабирования положения точки
    Point3D scalePoint(Point3f jointPoint, Point3f torso, double scale);
};

#endif // MAINWINDOW_H
