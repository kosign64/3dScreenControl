#include "mainwindow.h"
#include <QLayout>
#include <QLabel>
#include <QKeyEvent>
#include <QDebug>

// Конструктор класса
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Инициализация библиотеки NiTE2
    NiTE::initialize();
    // Создание трекера пользователя
    userTracker.create();

    // Создание главного виджета и его
    // настройка
    mainWidget = new QWidget(this);
    mainLayout = new QVBoxLayout;
    imageLabel = new QLabel(mainWidget);
    imageLabel->setFixedSize(640, 480);
    mainLayout->addWidget(imageLabel);
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);

    // Открытие последовательного порта порта
#ifdef __linux__
    port.openPort("/dev/ttyUSB0");
#elif defined(_WIN32) || defined(WIN32)
    port.openPort("COM3");
#endif
    // Если порт не открыт, то вывести об этом
    // сообщение
    if(!port.isOpened())
    {
        qDebug() << "Can't open port";
    }

    // Запуск таймера с минимально
    // возможным периудом
    startTimer(0);
}

// Деструктор класса
MainWindow::~MainWindow()
{
    // Завершение работы библиотеки
    // NiTE2
    NiTE::shutdown();
}

// Обработчик события таймера
void MainWindow::timerEvent(QTimerEvent *)
{
    // Массив для хранения изображения
    static uchar frameMas[320 * 240];
    UserTrackerFrameRef userTrackerFrame;
    userTracker.readFrame(&userTrackerFrame);
    // Получение изображения с камеры
    const VideoFrameRef &frame = userTrackerFrame.getDepthFrame();
    // Запись данных изображения в массив
    // (В Qt нет стандартного монохромного 16-и битного
    // изображения, только 8-и битное, поэтому также преобразуем
    // 16-и битные значения пикселей в 8-и битные)
    for(int i = 0; i < frame.getWidth(); ++i)
    {
        for(int j = 0; j < frame.getHeight(); ++j)
        {
            uint16_t pixelValue = *((uint16_t*)frame.getData() + i + j * 320);
            frameMas[i + j * 320] = (pixelValue >> 6);
        }
    }
    // Дальше по большей части ваш код
    const nite::Array<nite::UserData> &users = userTrackerFrame.getUsers();
    for (int i = 0; i < users.getSize(); ++i)
    {
        const nite::UserData &user = users[i];
        if (user.isNew())
        {
            qDebug() << "new user" << endl << flush;
            userTracker.startSkeletonTracking(user.getId());
        }
        else if (user.getSkeleton().getState() == nite::SKELETON_TRACKED)
        {

            const nite::SkeletonJoint& head =
                    user.getSkeleton().getJoint(nite::JOINT_HEAD);

            const nite::SkeletonJoint& neck =
                    user.getSkeleton().getJoint(nite::JOINT_NECK);

            const nite::SkeletonJoint& torso =
                    user.getSkeleton().getJoint(nite::JOINT_TORSO);

            const nite::SkeletonJoint& left_shoulder =
                    user.getSkeleton().getJoint(nite::JOINT_LEFT_SHOULDER);

            const nite::SkeletonJoint& right_shoulder =
                    user.getSkeleton().getJoint(nite::JOINT_RIGHT_SHOULDER);

            const nite::SkeletonJoint& left_elbow =
                    user.getSkeleton().getJoint(nite::JOINT_LEFT_ELBOW );

            const nite::SkeletonJoint& right_elbow =
                    user.getSkeleton().getJoint(nite::JOINT_RIGHT_ELBOW );

            const nite::SkeletonJoint& left_hand =
                    user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND);

            const nite::SkeletonJoint& right_hand =
                    user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND);

            nite::Point3f origin;
            origin.x = head.getPosition().x;
            origin.y = torso.getPosition().y;
            origin.z = head.getPosition().z;

            double k = 8 / (double)(head.getPosition().y - torso.getPosition().y);

            Point3D head3d = scalePoint(head.getPosition(),
                                        origin, k);
            Point3D rightHand3d = scalePoint(right_hand.getPosition(),
                                             origin, k);
            Point3D leftHand3d = scalePoint(left_hand.getPosition(),
                                            origin, k);
            Point3D rightElbow3d = scalePoint(right_elbow.getPosition(),
                                              origin, k);
            Point3D leftElbow3d = scalePoint(left_elbow.getPosition(),
                                             origin, k);
            Point3D rightShoulder3d = scalePoint(right_shoulder.getPosition(),
                                                 origin, k);
            Point3D leftShoulder3d = scalePoint(left_shoulder.getPosition(),
                                                origin, k);

            if (right_hand.getPositionConfidence() > .5)
            {
                uint8_t data[14] = {0};
                data[0] = head3d.angle << 1;
                if(head3d.x < 0) data[0] |= 1;
                data[1] = head3d.x << 4;
                data[1] |= head3d.y;

                data[2] = rightHand3d.angle << 1;
                if(rightHand3d.x < 0) data[2] |= 1;
                data[3] = rightHand3d.x << 4;
                data[3] |= head3d.y;

                data[4] = leftHand3d.angle << 1;
                if(leftHand3d.x < 0) data[4] |= 1;
                data[5] = leftHand3d.x << 4;
                data[5] |= leftHand3d.y;

                data[6] = rightElbow3d.angle << 1;
                if(rightElbow3d.x < 0) data[6] |= 1;
                data[7] = rightElbow3d.x << 4;
                data[7] |= rightElbow3d.y;

                data[8] = leftElbow3d.angle << 1;
                if(leftElbow3d.x < 0) data[8] |= 1;
                data[9] = leftElbow3d.x << 4;
                data[9] |= leftElbow3d.y;

                data[10] = rightShoulder3d.angle << 1;
                if(rightShoulder3d.x < 0) data[10] |= 1;
                data[11] = rightShoulder3d.x << 4;
                data[11] |= rightShoulder3d.y;

                data[12] = leftShoulder3d.angle << 1;
                if(leftShoulder3d.x < 0) data[12] |= 1;
                data[13] = leftShoulder3d.x << 4;
                data[13] |= leftShoulder3d.y;

                Point2D points[7];
                // Преобразование трёхмарных координат точек в двухмерные
                // координаты на карте глубины для отображения
                userTracker.convertJointCoordinatesToDepth(head.getPosition().x,
                                                           head.getPosition().y,
                                                           head.getPosition().z,
                                                           &points[0].x,
                                                           &points[0].y);
                userTracker.convertJointCoordinatesToDepth(left_elbow.getPosition().x,
                                                           left_elbow.getPosition().y,
                                                           left_elbow.getPosition().z,
                                                           &points[1].x,
                                                           &points[1].y);
                userTracker.convertJointCoordinatesToDepth(right_elbow.getPosition().x,
                                                           right_elbow.getPosition().y,
                                                           right_elbow.getPosition().z,
                                                           &points[2].x,
                                                           &points[2].y);
                userTracker.convertJointCoordinatesToDepth(left_shoulder.getPosition().x,
                                                           left_shoulder.getPosition().y,
                                                           left_shoulder.getPosition().z,
                                                           &points[3].x,
                                                           &points[3].y);
                userTracker.convertJointCoordinatesToDepth(right_shoulder.getPosition().x,
                                                           right_shoulder.getPosition().y,
                                                           right_shoulder.getPosition().z,
                                                           &points[4].x,
                                                           &points[4].y);
                userTracker.convertJointCoordinatesToDepth(left_hand.getPosition().x,
                                                           left_hand.getPosition().y,
                                                           left_hand.getPosition().z,
                                                           &points[5].x,
                                                           &points[5].y);
                userTracker.convertJointCoordinatesToDepth(right_hand.getPosition().x,
                                                           right_hand.getPosition().y,
                                                           right_hand.getPosition().z,
                                                           &points[6].x,
                                                           &points[6].y);

                qDebug() << "head x: " << head3d.x << "head_y: " <<
                            head3d.y << "head_ang " << head3d.angle;
                qDebug() << "Right hand x:" << rightHand3d.x <<
                            "Right hand y: " << rightHand3d.y <<
                            "right hand_ang " << rightHand3d.angle;

                // Добавление всех найденных точек на карту глубины
                for(size_t i = 0; i < (sizeof(points) / sizeof(points[0])); ++i)
                {
                    int index = (int)points[i].x + (int)points[i].y * 320;
                    if((index >= 0) && (index < (320 * 240)))
                    {
                        frameMas[index] = 255;
                    }
                }

                // Отправка данных на плату передатчика
                if(port.isOpened())
                {
                    port << 0xFF;
                    for(size_t i = 0; i < sizeof(data); ++i)
                    {
                        port << data[i];
                    }
                }
            }
        }
    }

    // Создание изображения из массива пикселей
    depthImage = QImage((const uchar*)frameMas, 320, 240, QImage::Format_Indexed8);
    // Масштабирование изображения до разрешения 640*480 и отображение его
    // на виджете
    imageLabel->setPixmap(QPixmap::fromImage(depthImage).scaled(640, 480,
                                                                Qt::KeepAspectRatio));
}

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    // Выход из программы при нажатии клавиши Esc
    if(ev->key() == Qt::Key_Escape)
    {
        close();
    }
}

// Ваша функция масштабирования точки
MainWindow::Point3D MainWindow::scalePoint(Point3f jointPoint, Point3f torso,
                                           double scale)
{
    Point3D point;
    Point3f res;
    double angle;

    res.x = (jointPoint.x - torso.x) * scale;
    res.y = (jointPoint.y - torso.y) * scale;
    res.z = (jointPoint.z - torso.z) * scale;


    double scale_ang = 128 / 360.;
    // В преобразовании угла у вас была ошибка и вы
    // использовали менее подходящую для этого функцию
    // atan
    double rad_ang = atan2(res.z, res.x);
    double vector = sqrt(pow(res.x, 2) + pow(res.z, 2));

    if (res.x > 0) vector = -vector;

    angle = rad_ang * 180 / M_PI;
    if(res.z < 0) angle = 360 + angle;
    angle *= scale_ang;

    point.angle = floor(angle + 0.5);
    point.x = floor(vector + 0.5);
    point.y = floor(res.y + 0.5);

    return point;
}
