#pragma once

#include <QAbstractListModel>
#include <QObject>
#include <QFile>
#include <QColor>
#include <QTextStream>
#include <QDebug>
#include <QJsonObject>
#include <QString>
#include <QJsonArray>
#include <QJsonDocument>
#include <cmath>
#include "bubble.h"

class Match3 : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(qint32 row MEMBER m_rows CONSTANT)
    Q_PROPERTY(qint32 col MEMBER m_columns CONSTANT)
    //Q_PROPERTY(qint32 score MEMBER m_score CONSTANT)
    //Q_PROPERTY(qint32 steps MEMBER m_steps CONSTANT)

public:
    explicit Match3(QObject *parent = nullptr);
    ~Match3();

    enum {
        ColorRole = Qt::UserRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;


public slots:
    void newGame();
    bool moveHandler(int clicked, int released);


private:
    int m_rows;
    int m_columns;
    //int m_score;
    //int m_steps;
    QList<Bubble*> m_bubbles;
    QList<QColor> m_colors;


    void setStartValues(const QString file);

    bool checkMove(int from, int to) const;
    int connectedBlocks(int row, int col, QColor color, QVector<int> &toDelete);
    void deleteBlocks();
    void move(int clicked, int released);
    void moveToBottom();
    void addNewBubbles();
    bool checkMatches();

};
