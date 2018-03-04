#ifndef VISUALISER_H
#define VISUALISER_H

#include <QWidget>
#include "shared.h"

class Visualiser : public QWidget
{
    Q_OBJECT
public:
    explicit Visualiser(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);

private:
    QVector<Point3D> m_points;

signals:

public slots:
    void getPoints(QVector<Point3D> points);
};

#endif // VISUALISER_H
