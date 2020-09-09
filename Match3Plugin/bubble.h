#pragma once

#include <QObject>
#include <QColor>

class Bubble : public QObject
{
    Q_OBJECT
public:
    explicit Bubble(QObject *parent = nullptr, QColor color = "black");

    QColor getColor() const;
    void setColor(QColor color);

private:
    QColor m_color;
};
