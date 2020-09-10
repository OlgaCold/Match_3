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
    return parent.isValid() ? 0 : m_rows * m_columns;
}


QVariant Match3::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    //int col = index.column();

    if (!index.isValid())
        return QVariant();

    switch(role) {
    case ColorRole:
        return m_bubbles.at(row)->getColor();
    /*case SizeRole:
        return m_bubbles.at(row)->getSize();*/
    }
    return QVariant();
}


QHash<int, QByteArray> Match3::roleNames() const
{
    return {
        { ColorRole, "color" },
        /*{SizeRole, "size"}*/ };
}

void Match3::clickHandler(int index)
{
    static int choosed = 0;
    static int first = -1;
    static int second = -1;

    choosed++;

    //beginResetModel();
    //m_bubbles.at(index)->setSize("small");
    //endResetModel();

    if(choosed == 1)
    {
        first = index;
    }

    if(choosed == 2)
    {
        second = index;
        choosed = 0;

        move(first, second);

        //m_bubbles.at(first)->setSize(100);
        //m_bubbles.at(second)->setSize(100);

        first = -1;
        second = -1;
    }
}

void Match3::move(int clicked, int released)
{
    //printf("clicked at %d, released at %d\n", clicked, released);

    if(released != -1 && clicked != released && checkMove(clicked, released)) {

        int min = clicked < released ? clicked : released;
        int max = clicked < released ? released : clicked;

        emit beginMoveRows(QModelIndex(), min, min, QModelIndex(), max + 1);
        m_bubbles.move(min, max);
        emit endMoveRows();

        if(max - 1 != min) {
            emit beginMoveRows(QModelIndex(), max - 1, max - 1, QModelIndex(), min);
            m_bubbles.move(max - 1, min);
            emit endMoveRows();
        }

        //return checkWin();
    }
}

void Match3::newGame()
{
    emit beginResetModel();
    for (int i = 0; i < m_bubbles.size(); i++){
        delete (m_bubbles.takeAt(i));
    }
    for(int i = 0; i < m_rows * m_columns; i++){

        m_bubbles.append(new Bubble(nullptr, m_colors.at(qrand() % m_colors.size())));
    }

    //std::random_shuffle(m_bubbles.begin(), m_bubbles.end());
    emit endResetModel();
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

    QJsonArray jsonArray = json_obj["colors"].toArray();

    for(int i = 0; i < jsonArray.size(); i++) {
        m_colors.append(jsonArray.at(i).toString());
    }

}

bool Match3::checkMove(int from, int to) const
{
    int from_row = floor(from / m_columns);
    int from_col = from % m_columns;
    int to_row = floor(to / m_columns);
    int to_col = to % m_columns;
    //printf("%d, %d  %d, %d\n", from_row, from_col, to_row, to_col);

    if( (abs(from_row - to_row) == 1 && abs(from_col - to_col) == 0) ||
        (abs(from_col - to_col) == 1 && abs(from_row - to_row) == 0)){
        return true;
    } else { return false; }

}

/*void Match3::connectedBlocks(int clicked, int released)
{
    int from_row = floor(from / m_columns);
    int from_col = from % m_columns;
    int to_row = floor(to / m_columns);
    int to_col = to % m_columns;

    static row_connected = 1;
    static col_connected = 1;



}*/

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


QModelIndex GameMatch3::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
}*/
