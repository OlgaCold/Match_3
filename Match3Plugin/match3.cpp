#include "match3.h"

Match3::Match3(QObject *parent)
    : QAbstractListModel(parent)
{

    setStartValues("start_values.json");

    for(int i = 0; i < m_rows * m_columns; i++){

        m_bubbles.append(new Bubble(nullptr, m_colors.at(qrand() % m_colors.size()), false));
    }

    //printf("%d ", connectedBlocks(3));
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

/*void Match3::clickHandler(int index)
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
}*/

void Match3::move(int clicked, int released)
{
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
    }

}

bool Match3::moveHandler(int clicked, int released)
{
    //printf("clicked at %d, released at %d\n", clicked, released);

    int row_1 = floor(clicked / m_columns);
    int col_1 = clicked % m_columns;
    int row_2 = floor(released / m_columns);
    int col_2 = released % m_columns;
    QVector<int> toDelete;

    int clickedConnected = 0;
    int releasedConnected = 0;

    move(clicked, released);

    //printf("%d ", connectedBlocks(row_2, col_2, m_bubbles.at(released)->getColor()));
    //printf("%d\n",connectedBlocks(row_1, col_1, m_bubbles.at(clicked)->getColor()));


    clickedConnected = connectedBlocks(row_1, col_1, m_bubbles.at(clicked)->getColor(), toDelete);

    if (clickedConnected >= 3) {
        for(int i = 0; i < toDelete.count(); i++){

            m_bubbles.at(toDelete.at(i))->setMarkedToDelete(true);
        }
    }
    toDelete.clear();

    releasedConnected = connectedBlocks(row_2, col_2, m_bubbles.at(released)->getColor(), toDelete);
    if (releasedConnected >= 3) {
        for(int i = 0; i < toDelete.count(); i++){
            m_bubbles.at(toDelete.at(i))->setMarkedToDelete(true);
        }
    }
    toDelete.clear();

    if( clickedConnected >= 3 || releasedConnected >= 3 ) {

        deleteBlocks();


        for(int i = 0; i < m_bubbles.count(); i++){
            m_bubbles.at(i)->setMarkedToDelete(false);
        }
        moveToBottom();
        return true;

    } else {

        move(clicked, released);

        for(int i = 0; i < m_bubbles.count(); i++){
            m_bubbles.at(i)->setMarkedToDelete(false);
        }
        return false;
    }
}


void Match3::newGame()
{
    emit beginResetModel();
    for (int i = 0; i < m_bubbles.size(); i++){
        delete (m_bubbles.takeAt(i));
    }
    for(int i = 0; i < m_rows * m_columns; i++){

        m_bubbles.append(new Bubble(nullptr, m_colors.at(qrand() % m_colors.size()), false));
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

int Match3::connectedBlocks(int row, int col, QColor color, QVector<int> &toDelete)
{

    if(row >= m_rows || col >= m_columns || row < 0 || col < 0)
    {
        return 0; }

    if(toDelete.contains(row * m_columns + col))
    {
        return 0; }

    if(m_bubbles.at(row * m_columns + col)->getColor() != color)
    {
        return 0; }

    int connected_blocks = 1;
    toDelete.append(row * m_columns + col);

    //m_bubbles.at(row * m_columns + col)->setMarkedToDelete(true);
    //printf("%d , %d ", row, col);
    //printf(m_bubbles.at(row * m_columns + col)->getMarkedToDelete() ? "true " : "false ");


    connected_blocks += connectedBlocks(row + 1, col, color, toDelete);
    connected_blocks += connectedBlocks(row - 1, col, color, toDelete);
    connected_blocks += connectedBlocks(row, col + 1, color, toDelete);
    connected_blocks += connectedBlocks(row, col - 1, color, toDelete);

    return connected_blocks;

}

void Match3::deleteBlocks()
{
    for(int i = 0; i < m_rows; i++){
        for(int j = 0; j < m_columns; j++){
            if(m_bubbles.at(i * m_columns + j)->getMarkedToDelete()){
                emit beginRemoveRows(QModelIndex(), i * m_columns + j, i * m_columns + j);
                m_bubbles.removeAt(i * m_columns + j);
                //printf(m_bubbles.at(i * m_columns + j)->getMarkedToDelete() ? "true " : "false ");
                emit endRemoveRows();
                emit beginInsertRows(QModelIndex(), i * m_columns + j, i * m_columns + j);
                m_bubbles.insert(i * m_columns + j, new Bubble(nullptr, "transparent", false));
                emit endInsertRows();
            }
        }
    }

    //emit beginRemoveRows(QModelIndex(), 3, 3);
    //m_bubbles.removeAt(3);
    //emit endRemoveRows();
    //emit beginInsertRows(QModelIndex(), 3, 3);
    //m_bubbles.insert(3, new Bubble(nullptr, m_colors.at(qrand() % m_colors.size()), false));
    //emit endInsertRows();


}

void Match3::moveToBottom()
{

    for(int col = 0; col < m_columns; col++){
        for(int row = m_rows - 1; row >= 0; row--){
            if(m_bubbles.at(row * m_columns + col)->getColor() == "transparent") {

                int tempInd = row * m_columns + col - m_columns;

                while(tempInd >= 0 && m_bubbles.at(row * m_columns + col)->getColor() == "transparent"){
                    if(m_bubbles.at(tempInd)->getColor() != "transparent"){
                        move(row * m_columns + col, tempInd);
                        tempInd -= m_columns;
                    } else {
                        tempInd -= m_columns;
                    }
                }
            }
        }
    }
    /*for(int ind = m_bubbles.count() - 1; ind >= 0; ind--) {
        //printf("here");
        if(m_bubbles.at(ind)->getColor() == "transparent") {
            //printf("here1");
            int tempInd = ind - m_columns;

            while(tempInd >= 0){
                //printf("here2");
                if(m_bubbles.at(tempInd)->getColor() != "transparent"){
                    move(ind, tempInd);
                    tempInd -= m_columns;
                } else {
                    tempInd -= m_columns;
                }
            }
        }
    }*/
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


QModelIndex GameMatch3::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
}*/
