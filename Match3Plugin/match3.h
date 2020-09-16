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
    Q_PROPERTY(qint32 steps MEMBER m_steps NOTIFY stepsChanged)
    Q_PROPERTY(qint32 score MEMBER m_score NOTIFY scoreChanged)


public:
    explicit Match3(int steps = 0, int score = 0, QObject *parent = nullptr);
    ~Match3() override;

    enum {
        ColorRole = Qt::UserRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

signals:
   void stepsChanged(int i);
   void scoreChanged(int i);

public slots:
    bool newGame(); //return false if no move available
    bool moveHandler(int clicked, int released);
    bool checkAvailableSteps();


private:
    int m_rows;
    int m_columns;
    int m_steps;
    int m_score;
    QList<Bubble*> m_bubbles;
    QList<QColor> m_colors;


    void setStartValues(const QString file);

    bool checkMove(int from, int to) const;
    int connectedBlocks(int row, int col, QColor color, QVector<int> &toDelete);
    int connectedBlocks(int row, int col, QColor color, QVector<int> &toDelete, QList<Bubble*> &board);

    void deleteBlocks();
    void move(int clicked, int released);
    void moveToBottom();
    void addNewBubbles();
    bool checkMatches(); //return true if match exist and mark bubbles to delete

};
