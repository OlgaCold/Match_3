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

public:
    explicit Match3(QObject *parent = nullptr);
    ~Match3() override;

    enum {
        ColorRole = Qt::UserRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

public slots:
    bool newGame(); //return false if no move available
    bool moveHandler(int clicked, int released);
    bool checkAvailableSteps(); //brute force algorithm, return true if step available

    bool checkMatches(); //return true if match exist and mark bubbles to delete
    int deleteBlocks(); //remove and insert transparent blocks by marks
    void moveToBottom(); //move transparent up
    void addNewBubbles(); //delete transparent and insert random color bubble
    void clearBubbleMarks(); // set all Bubble m_markedToDelete to false

private:
    int m_rows;
    int m_columns;
    QList<Bubble*> m_bubbles;
    QList<QColor> m_colors;

    bool configFromJson(const QString path);
    void createStandartJson(const QString path);
    bool checkMove(int from, int to) const; // return true, if gamer move is valid
    int connectedBlocks(int row, int col, QColor color, QVector<int> &toDelete); // return number of connected blocks and fill vectop toDelete
    int connectedBlocks(int row, int col, QColor color, QVector<int> &toDelete, QList<Bubble*> &board); //for copy of the Board
    int connected(int row, int col, QColor color, QVector<int> &toDelete);
    void fillField();
    QList<Bubble*> copyBoard();
    void deleteBoardCopy(QList<Bubble*> &board);
    void move(int clicked, int released);
    int row(int index) const;
    int col(int index) const;
    int getIndex(int row, int col) const;
};
