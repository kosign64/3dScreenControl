#include "visualiser.h"
#include <QPainter>

Visualiser::Visualiser(QWidget *parent) : QWidget(parent)
{

}

void Visualiser::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.translate(width() / 2, height() / 2);
    painter.setBrush(QBrush(Qt::black));
    painter.drawEllipse(-width() / 2, -height() / 2, width(), height());
    painter.setBrush(QBrush(Qt::red));
    foreach (Point3D point, m_points)
    {
        painter.save();
        double angle = point.angle * 360. / 128.;
        double scale = (width() / 32.) * 0.9;
        painter.rotate(angle);
        painter.drawEllipse(point.x * scale, 0, width() * 0.1,
                            height() * 0.1);
        painter.restore();
    }
}

void Visualiser::getPoints(QVector<Point3D> points)
{
    m_points = points;
    update();
}
