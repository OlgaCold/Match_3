#pragma once

#include <QObject>
#include <QColor>


class Bubble : public QObject
{
    Q_OBJECT
public:
    explicit Bubble(QObject *parent = nullptr, QColor color = "black", bool markedToDelete = false);
    //Bubble(const Bubble &bubble);

    QColor getColor() const;
    void setColor(QColor color);

    bool getMarkedToDelete() const;
    void setMarkedToDelete(bool markedToDelete);

private:
    QColor m_color;
    bool m_markedToDelete;
};
