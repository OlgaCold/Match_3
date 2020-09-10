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
    //int getSize() const;
    //void setSize(int size);

private:
    QColor m_color;
    //int m_size;
};
