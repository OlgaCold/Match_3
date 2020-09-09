#pragma once

#include <QAbstractListModel>
#include <QObject>
#include <QFile>
#include <QColor>
#include <QTextStream>
#include <QDebug>
#include <QJsonObject>
#include <QString>
//#include <QJsonValue>
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
    ~Match3();

    enum {
        ColorRole = Qt::UserRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    //int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    // Basic functionality:
    //QModelIndex index(int row, int column,
    //                 const QModelIndex &parent = QModelIndex()) const override;
    //QModelIndex parent(const QModelIndex &index) const override;*/

    /*// Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;*/
public slots:
    void move(int clicked, int released);
    void newGame();

private:
    int m_rows;
    int m_columns;
    QList<Bubble*> m_bubbles;
    QList<QColor> m_colors;

    void setStartValues(const QString file);
    bool checkMove(int from, int to) const;

};
