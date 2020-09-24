#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDebug>
#include <cmath>
#include <QColor>
#include <QAbstractListModel>
#include "match3.h"

struct Bubble
{
    Bubble(QColor color = "black", bool markedToDelete = false) :
        m_color(color), m_markedToDelete(markedToDelete) {
    }

    QColor m_color;
    bool m_markedToDelete;
};

Match3::Match3(QObject *parent)
    : QAbstractListModel(parent)
{
    if(!configFromJson("config.json")){
        createStandartJson("config.json");
        configFromJson("config.json");
    }

    fillField();
    while(!checkAvailableSteps()){
        fillField();
    }
}

Match3::~Match3()
{
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

    if (!index.isValid())
        return QVariant();

    switch(role) {
    case ColorRole:
        return m_bubbles.at(row)->m_color;
    }

    return QVariant();
}

QHash<int, QByteArray> Match3::roleNames() const
{
    return { { ColorRole, "color" } };
}

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
    int row_1 = row(clicked);
    int col_1 = col(clicked);
    int row_2 = row(released);
    int col_2 = col(released);

    QVector<int> toDelete;

    int clickedConnected = 0;
    int releasedConnected = 0;

    move(clicked, released);

    clickedConnected = connectedBlocks(row_1, col_1, m_bubbles.at(clicked)->m_color, toDelete, m_bubbles);

    if (clickedConnected >= 3) {
        for(int i = 0; i < toDelete.count(); i++){
            m_bubbles.at(toDelete.at(i))->m_markedToDelete = true;
        }
    }
    toDelete.clear();

    releasedConnected = connectedBlocks(row_2, col_2, m_bubbles.at(released)->m_color, toDelete, m_bubbles);
    if (releasedConnected >= 3) {
        for(int i = 0; i < toDelete.count(); i++){
            m_bubbles.at(toDelete.at(i))->m_markedToDelete = true;
        }
    }
    toDelete.clear();

    if( clickedConnected >= 3 || releasedConnected >= 3 ) {

        return true;

    } else {
        move(clicked, released);
        clearBubbleMarks();
        return false;
    }
}

bool Match3::checkAvailableSteps()
{
    for(int i = 0; i < m_rows; i++){
        for(int j = 0; j < m_columns; j++){

            if(i >= m_rows - 1){
                continue;
            }

            QList<Bubble*> boardCopy = copyBoard();
            QVector<int> toDelete;

            int curIndex = i * m_columns + j;
            int swapIndex = (i + 1) * m_columns + j;

            int min = curIndex < swapIndex ? curIndex : swapIndex;
            int max = curIndex < swapIndex ? swapIndex : curIndex;

            boardCopy.move(min, max);
            if(max - 1 != min) {
                boardCopy.move(max - 1, min);
            }

            int clickedConnected = connectedBlocks(i, j, boardCopy.at(curIndex)->m_color, toDelete, boardCopy);

            toDelete.clear();
            int releasedConnected = connectedBlocks(i + 1, j, boardCopy.at(swapIndex)->m_color, toDelete, boardCopy);
            toDelete.clear();

            deleteBoard(boardCopy);

            if( clickedConnected >= 3 || releasedConnected >= 3 ) {
                return true;
            }
        }
    }

    for(int i = 0; i < m_rows; i++){
        for(int j = 0; j < m_columns; j++){

            if(j >= m_columns - 1){
                continue;
            }

            QList<Bubble*> boardCopy = copyBoard();
            QVector<int> toDelete;

            int curIndex = i * m_columns + j;
            int swapIndex = i * m_columns + (j + 1);

            int min = curIndex < swapIndex ? curIndex : swapIndex;
            int max = curIndex < swapIndex ? swapIndex : curIndex;

            boardCopy.move(min, max);
            if(max - 1 != min) {
                boardCopy.move(max - 1, min);
            }

            int clickedConnected = connectedBlocks(i, j, boardCopy.at(curIndex)->m_color/*->getColor()*/, toDelete, boardCopy);

            toDelete.clear();
            int releasedConnected = connectedBlocks(i, j + 1, boardCopy.at(swapIndex)->m_color/*->getColor()*/, toDelete, boardCopy);
            toDelete.clear();

            deleteBoard(boardCopy);

            if( clickedConnected >= 3 || releasedConnected >= 3 ) {
                return true;
            }
        }
    }

    return false;
}

bool Match3::newGame()
{
    fillField();

    if(!checkAvailableSteps()){
        return false;
    }
    return true;
}

bool Match3::configFromJson(const QString path)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<< "Failed to open JSON. Created new JSON file." << path;
        return false;
    }

    QString jsonString = file.readAll();
    file.close();
    QByteArray json_bytes = jsonString.toLocal8Bit();

    auto jsonDoc = QJsonDocument::fromJson(json_bytes);

    if(jsonDoc.isNull()){
        qDebug()<<"Failed to create QJsonDocument. Created new JSON file.";
        return false;
    }

    if(!jsonDoc.isObject()){
        qDebug()<<"JSON is not an object. Created standart.";
        return false;
    }

    QJsonObject jsonObj = jsonDoc.object();

    if(jsonObj.isEmpty()){
        qDebug()<<"JSON object is empty. Created standart.";
        return false;
    }

    m_rows = jsonObj.value("rows").toInt();
    m_columns = jsonObj.value("columns").toInt();
    QJsonArray jsonArray = jsonObj.value("colors").toArray();

    for(int i = 0; i < jsonArray.size(); i++) {
        m_colors.append(jsonArray.at(i).toString());
    }
    return true;
}

void Match3::createStandartJson(const QString path)
{
    QJsonObject jsonObj;
    QJsonArray jsonArray = {"black", "green", "blue", "white", "red", "orange", "violet"};
    jsonObj["columns"] = 5;
    jsonObj["rows"] = 5;
    jsonObj["colors"] = jsonArray;

    QJsonDocument jsonDoc(jsonObj);
    QString jsonString = jsonDoc.toJson();

    if(QFile::exists(path)){
        QFile::remove(path);
    }

    QFile file(path);
    if(!file.open(QIODevice::ReadWrite)){
        qDebug() << "Failed to open save file.";
        exit(1);
    }

    file.write(jsonString.toLocal8Bit());
    file.close();
}

bool Match3::checkMove(int from, int to) const
{
    int from_row = row(from);
    int from_col = col(from);
    int to_row = row(to);
    int to_col = col(to);

    if( (abs(from_row - to_row) == 1 && abs(from_col - to_col) == 0) ||
            (abs(from_col - to_col) == 1 && abs(from_row - to_row) == 0)){
        return true;
    } else
    { return false; }
}

int Match3::connectedBlocks(int row, int col, QColor color, QVector<int> &toDelete, QList<Bubble *> &board)
{
    if(row >= m_rows || col >= m_columns || row < 0 || col < 0)
    {
        return 0; }

    if(toDelete.contains(getIndex(row, col)))
    {
        return 0; }

    if(board.at(getIndex(row, col))->m_color != color)
    {
        return 0; }


    int connected_blocks = 1;
    toDelete.append(getIndex(row, col));

    connected_blocks += connectedBlocks(row + 1, col, color, toDelete, board);
    connected_blocks += connectedBlocks(row - 1, col, color, toDelete, board);
    connected_blocks += connectedBlocks(row, col + 1, color, toDelete, board);
    connected_blocks += connectedBlocks(row, col - 1, color, toDelete, board);

    return connected_blocks;
}

void Match3::fillField()
{
    emit beginResetModel();

    if(!m_bubbles.isEmpty()){
        deleteBoard(m_bubbles);
    }
    for(int i = 0; i < m_rows * m_columns; i++) {
        m_bubbles.append(new Bubble(m_colors.at(qrand() % m_colors.size()), false));
    }


    for(int i = 0; i < m_rows; i++){
        for(int j = 0; j < m_columns; j++){
            QVector<int> vector;
            QColor color = m_bubbles.at(getIndex(i, j))->m_color;
            while(connectedBlocks(i, j, color, vector, m_bubbles) >= 3){
                color = m_colors.at(qrand() % m_colors.size());
                m_bubbles.at(getIndex(i, j))->m_color = color;
                vector.clear();
            }
        }
    }
    emit endResetModel();
}

QList<Bubble *> Match3::copyBoard()
{
    QList<Bubble*> boardCopy;
    for(int i = 0; i < m_bubbles.count(); i++){
        boardCopy.append(new Bubble(m_bubbles.at(i)->m_color, false));
    }
    return boardCopy;
}

void Match3::deleteBoard(QList<Bubble *> &board)
{
    for (int i = 0; i < board.size(); i++){
        delete board.takeAt(i);
    }
}

int Match3::deleteBlocks()
{
    int deleted = 0;

    for(int i = 0; i < m_rows; i++){
        for(int j = 0; j < m_columns; j++){
            if(m_bubbles.at(i * m_columns + j)->m_markedToDelete){

                emit beginRemoveRows(QModelIndex(), i * m_columns + j, i * m_columns + j);
                m_bubbles.removeAt(i * m_columns + j);
                emit endRemoveRows();

                emit beginInsertRows(QModelIndex(), i * m_columns + j, i * m_columns + j);
                m_bubbles.insert(i * m_columns + j, new Bubble("transparent", false));
                emit endInsertRows();

                deleted += 1;
            }
        }
    }

    return deleted;
}

void Match3::moveToBottom()
{

    for(int col = 0; col < m_columns; col++){
        for(int row = m_rows - 1; row >= 0; row--){
            if(m_bubbles.at(getIndex(row, col))->m_color == "transparent") {

                int ind = getIndex(row, col);
                int tempInd = getIndex(row, col) - m_columns;
                while(tempInd >= 0){
                    if(m_bubbles.at(tempInd)->m_color != "transparent"){

                        //not called move function, because move can be not by rules
                        int min = ind < tempInd ? ind : tempInd;
                        int max = ind < tempInd ? tempInd : ind;

                        emit beginMoveRows(QModelIndex(), min, min, QModelIndex(), max + 1);
                        m_bubbles.move(min, max);
                        emit endMoveRows();

                        if(max - 1 != min) {
                            emit beginMoveRows(QModelIndex(), max - 1, max - 1, QModelIndex(), min);
                            m_bubbles.move(max - 1, min);
                            emit endMoveRows();
                        }//

                        tempInd -= m_columns;
                        ind -= m_columns;
                    } else {
                        tempInd -= m_columns;
                    }
                }
            }
        }
    }
}

void Match3::addNewBubbles()
{
    for(int col = 0; col < m_columns; col++){
        while(m_bubbles.at(0 * m_columns + col)->m_color == "transparent") {

            emit beginRemoveRows(QModelIndex(), 0 * m_columns + col, 0 * m_columns + col);
            m_bubbles.removeAt(0 * m_columns + col);
            emit endRemoveRows();

            emit beginInsertRows(QModelIndex(), 0 * m_columns + col, 0 * m_columns + col);
            m_bubbles.insert(0 * m_columns + col, new Bubble(m_colors.at(qrand() % m_colors.size()), false));
            emit endInsertRows();

            moveToBottom();
        }
    }
}

bool Match3::checkMatches()
{
    bool matchExist = false;

    for(int i = 0; i < m_bubbles.count(); i++){
        if(m_bubbles.at(i)->m_markedToDelete == false){

            QColor color = m_bubbles.at(i)->m_color;
            QVector<int> onDelete;
            int temp_row = row(i);
            int temp_col = col(i);

            int connected = connectedBlocks(temp_row, temp_col, color, onDelete, m_bubbles);

            if(connected >= 3){
                matchExist = true;

                for(int i = 0; i < onDelete.count(); i++){
                    m_bubbles.at(onDelete.at(i))->m_markedToDelete = true;
                }

                onDelete.clear();
            }
        }
    }
    return matchExist;
}

void Match3::clearBubbleMarks()
{
    for(int i = 0; i < m_bubbles.count(); i++){
        m_bubbles.at(i)->m_markedToDelete = false;
    }
}

int Match3::deleteHandler()
{
    int deleted = deleteBlocks();
    clearBubbleMarks();
    moveToBottom();
    addNewBubbles();

    return deleted;
}

int Match3::row(int index) const
{
    return floor(index / m_columns);
}

int Match3::col(int index) const
{
    return index % m_columns;
}

int Match3::getIndex(int row, int col) const
{
    return row * m_columns + col;
}

