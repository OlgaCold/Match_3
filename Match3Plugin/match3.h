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
    bool checkAvailableSteps(); //brute force algorithm, return true if step available

    bool checkMatches(); //return true if match exist and mark bubbles to delete
    void deleteBlocks(); //remove and insert transparent blocks by marks
    void moveToBottom(); //move transparent up
    void addNewBubbles(); //delete transparent and insert random color bubble
    void clearBubbleMarks(); // set all Bubble m_markedToDelete to false

private:
    int m_rows;
    int m_columns;
    int m_steps;
    int m_score;
    QList<Bubble*> m_bubbles;
    QList<QColor> m_colors;


    bool configFromJson(const QString path);
    void createStandartJson(const QString path);
    bool checkMove(int from, int to) const; // return true, if gamer move is valid
    int connectedBlocks(int row, int col, QColor color, QVector<int> &toDelete); // return number of connected blocks and fill vectop toDelete
    int connectedBlocks(int row, int col, QColor color, QVector<int> &toDelete, QList<Bubble*> &board); //for copy of the Board
    void move(int clicked, int released);  
    int row(int index) const;
    int col(int index) const;
};
