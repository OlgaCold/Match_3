#include "bubble.h"

Bubble::Bubble(QObject *parent, QColor color, bool markedToDelete) : QObject(parent),
    m_color(color), m_markedToDelete(markedToDelete) { }

/*Bubble::Bubble(const Bubble &bubble) : QObject(nullptr),
    m_color(bubble.m_color), m_markedToDelete(bubble.m_markedToDelete) { }*/

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

