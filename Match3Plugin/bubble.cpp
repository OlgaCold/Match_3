#include "bubble.h"

Bubble::Bubble(QObject *parent, QColor color) : QObject(parent), m_color(color)
{

}

QColor Bubble::getColor() const
{
    return m_color;
}

void Bubble::setColor(QColor color)
{
    m_color = color;
}
