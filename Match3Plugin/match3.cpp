#include "match3.h"

Match3::Match3(QObject *parent)
    : QAbstractListModel(parent)
{

    setStartValues("start_values.json");

    for(int i = 0; i < m_rows * m_columns; i++){

        m_bubbles.append(new Bubble(nullptr, m_colors.at(qrand() % m_colors.size())));
    }
}

Match3::~Match3(){
    for (int i = 0; i < m_bubbles.size(); i++){
        delete (m_bubbles.takeAt(i));
    }
}

int Match3::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_bubbles.count();
}


QVariant Match3::data(const QModelIndex &index, int role) const
{
    int row = index.row();

    if (!index.isValid())
        return QVariant();

    switch(role) {
    case ColorRole:
        return m_bubbles.at(row)->getColor();
    }
    return QVariant();
}

QHash<int, QByteArray> Match3::roleNames() const
{
    return {{ ColorRole, "color" }};
}

void Match3::setStartValues(const QString file)
{
    QFile file_obj(file);
    if(!file_obj.open(QIODevice::ReadOnly)){
        qDebug()<<"Failed to open "<<file;
        exit(1);
    }

    QTextStream file_text(&file_obj);
    QString json_string;
    json_string = file_text.readAll();
    file_obj.close();
    QByteArray json_bytes = json_string.toLocal8Bit();

    auto json_doc = QJsonDocument::fromJson(json_bytes);

    if(json_doc.isNull()){
        qDebug()<<"Failed to create JSON doc.";
        exit(2);
    }
    if(!json_doc.isObject()){
        qDebug()<<"JSON is not an object.";
        exit(3);
    }

    QJsonObject json_obj = json_doc.object();

    if(json_obj.isEmpty()){
        qDebug()<<"JSON object is empty.";
        exit(4);
    }

    QVariantMap json_map = json_obj.toVariantMap();

    m_rows = json_map["rows"].toInt();
    m_columns = json_map["columns"].toInt();
    //qDebug()<< json_map["rows"].toInt();
    //qDebug()<< json_map["columns"].toInt();

    QJsonArray jsonArray = json_obj["colors"].toArray();

    for(int i = 0; i < jsonArray.size(); i++) {
        m_colors.append(jsonArray.at(i).toString());
        //qDebug()<< m_colors.at(i);
    }

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
