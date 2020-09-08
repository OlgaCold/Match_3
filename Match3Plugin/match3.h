#pragma once

#include <QQuickItem>

class Match3 : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(Match3)

public:
    Match3(QQuickItem *parent = nullptr);
    ~Match3();
};

