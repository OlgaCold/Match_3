#include "bubble.h"

Bubble::Bubble(QObject *parent, QColor color, bool markedToDelete) : QObject(parent),
    m_color(color), m_markedToDelete(markedToDelete) { }

QColor Bubble::getColor() const
{
    return m_color;
}

void Bubble::setColor(QColor color)
{
    m_color = color;
}

bool Bubble::getMarkedToDelete() const
{
    return m_markedToDelete;
}

void Bubble::setMarkedToDelete(bool markedToDelete)
{
    m_markedToDelete = markedToDelete;
}

