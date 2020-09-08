#include "gamematch3.h"

Match3::Match3(QObject *parent)
    : QAbstractListModel(parent)
{
    for(int i = 0; i < 12; i++){
        m_bubbles.append(new Bubble(nullptr, "#6495ed"));
    }
}

int Match3::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    return m_bubbles.count();
}


QVariant Match3::data(const QModelIndex &index, int role) const
{
    int row = index.row();

    if (!index.isValid())
        return QVariant();

    switch(role) {
    case DecorationRole:
        return m_bubbles.at(row)->getColor();
    }
    return QVariant();
}

QHash<int, QByteArray> Match3::roleNames() const
{
    return {{ DecorationRole, "decoration" }};
}


/*

bool GameMatch3::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
}

bool GameMatch3::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endInsertColumns();
}

bool GameMatch3::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
}

bool GameMatch3::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endRemoveColumns();
}
QModelIndex GameMatch3::index(int row, int column, const QModelIndex &parent) const
{
    // FIXME: Implement me!
}

QModelIndex GameMatch3::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
}*/
