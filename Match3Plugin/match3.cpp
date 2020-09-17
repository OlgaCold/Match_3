#include "match3.h"

Match3::Match3(int steps, int score, QObject *parent)
    : QAbstractListModel(parent), m_steps(steps), m_score(score)
{

    readStartValues("start_values.json");

    for(int i = 0; i < m_rows * m_columns; i++) {
        m_bubbles.append(new Bubble(nullptr, m_colors.at(qrand() % m_colors.size()), false));
    }

    emit beginResetModel();
    while(checkMatches()){
        deleteBlocks();
        moveToBottom();
        addNewBubbles();
    }
    printf("Constructor deleted matches\n");
    emit endResetModel();
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
        return m_bubbles.at(row)->getColor();
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

    clickedConnected = connectedBlocks(row_1, col_1, m_bubbles.at(clicked)->getColor(), toDelete);

    if (clickedConnected >= 3) {
        for(int i = 0; i < toDelete.count(); i++){
            m_bubbles.at(toDelete.at(i))->setMarkedToDelete(true);
        }
        //m_score += clickedConnected;
    }
    toDelete.clear();

    releasedConnected = connectedBlocks(row_2, col_2, m_bubbles.at(released)->getColor(), toDelete);
    if (releasedConnected >= 3) {
        for(int i = 0; i < toDelete.count(); i++){
            m_bubbles.at(toDelete.at(i))->setMarkedToDelete(true);
        }
        //m_score += releasedConnected;
    }
    toDelete.clear();

    if( clickedConnected >= 3 || releasedConnected >= 3 ) {

        //deleteBlocks();
        //clearBubbleMarks();

        m_steps++;
        emit stepsChanged(m_steps);
        //emit scoreChanged(m_score);


        //moveToBottom();
        //addNewBubbles();
        //clearBubbleMarks();

        /*while(checkMatches()){
            deleteBlocks();
            clearBubbleMarks();
            moveToBottom();
            addNewBubbles();
            //m_steps++;
            //emit stepsChanged(m_steps);
        }*/
        //m_steps = 0;
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

            QList<Bubble*> boardCopy;
            for(int i = 0; i < m_bubbles.count(); i++){
                boardCopy.append(new Bubble(nullptr, m_bubbles.at(i)->getColor(), false));//
            }
            QVector<int> toDelete;

            int curIndex = i * m_columns + j;
            int swapIndex = (i + 1) * m_columns + j;

            int min = curIndex < swapIndex ? curIndex : swapIndex;
            int max = curIndex < swapIndex ? swapIndex : curIndex;

            boardCopy.move(min, max);
            if(max - 1 != min) {
                boardCopy.move(max - 1, min);
            }

            //printf("current - %d, swap - %d\n", curIndex, swapIndex);

            int clickedConnected = connectedBlocks(i, j, boardCopy.at(curIndex)->getColor(), toDelete, boardCopy);

            toDelete.clear();
            int releasedConnected = connectedBlocks(i + 1, j, boardCopy.at(swapIndex)->getColor(), toDelete, boardCopy);
            toDelete.clear();
            //printf("1 connect - %d, 2 connect - %d\n", clickedConnected, releasedConnected);

            for (int i = 0; i < boardCopy.size(); i++){
                delete boardCopy.takeAt(i);
            }

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

            QList<Bubble*> boardCopy;
            for(int i = 0; i < m_bubbles.count(); i++){
                boardCopy.append(new Bubble(nullptr, m_bubbles.at(i)->getColor(), false));//
            }
            QVector<int> toDelete;

            int curIndex = i * m_columns + j;
            int swapIndex = i * m_columns + (j + 1);

            int min = curIndex < swapIndex ? curIndex : swapIndex;
            int max = curIndex < swapIndex ? swapIndex : curIndex;

            boardCopy.move(min, max);
            if(max - 1 != min) {
                boardCopy.move(max - 1, min);
            }

            //printf("current - %d, swap - %d\n", curIndex, swapIndex);

            int clickedConnected = connectedBlocks(i, j, boardCopy.at(curIndex)->getColor(), toDelete, boardCopy);

            toDelete.clear();
            int releasedConnected = connectedBlocks(i, j + 1, boardCopy.at(swapIndex)->getColor(), toDelete, boardCopy);
            toDelete.clear();
            //printf("1 connect - %d, 2 connect - %d\n", clickedConnected, releasedConnected);

            for (int i = 0; i < boardCopy.size(); i++){
                delete boardCopy.takeAt(i);
            }

            if( clickedConnected >= 3 || releasedConnected >= 3 ) {
                return true;
            }
        }
    }

    return false;
}
bool Match3::newGame()
{
    emit beginResetModel();
    for (int i = 0; i < m_bubbles.size(); i++){
        m_bubbles.at(i)->setColor(m_colors.at(qrand() % m_colors.size()));
    }
    while(checkMatches()){
        deleteBlocks();//
        moveToBottom();
        addNewBubbles();
    }
    //printf("New game deleted matches\n");
    emit endResetModel();

    m_steps = 0;
    m_score = 0;
    emit stepsChanged(m_steps);
    emit scoreChanged(m_score);

    if(!checkAvailableSteps()){
        return false;
    }
    return true;
}
void Match3::readStartValues(const QString file)
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
    int from_row = row(from);
    int from_col = col(from);
    int to_row = row(to);
    int to_col = col(to);

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

    connected_blocks += connectedBlocks(row + 1, col, color, toDelete);
    connected_blocks += connectedBlocks(row - 1, col, color, toDelete);
    connected_blocks += connectedBlocks(row, col + 1, color, toDelete);
    connected_blocks += connectedBlocks(row, col - 1, color, toDelete);

    return connected_blocks;

}
int Match3::connectedBlocks(int row, int col, QColor color, QVector<int> &toDelete, QList<Bubble *> &board)
{
    if(row >= m_rows || col >= m_columns || row < 0 || col < 0)
    {
        return 0; }

    if(toDelete.contains(row * m_columns + col))
    {
        return 0; }

    if(board.at(row * m_columns + col)->getColor() != color)
    {
        return 0; }

    int connected_blocks = 1;
    toDelete.append(row * m_columns + col);

    connected_blocks += connectedBlocks(row + 1, col, color, toDelete, board);
    connected_blocks += connectedBlocks(row - 1, col, color, toDelete, board);
    connected_blocks += connectedBlocks(row, col + 1, color, toDelete, board);
    connected_blocks += connectedBlocks(row, col - 1, color, toDelete, board);

    return connected_blocks;
}
void Match3::deleteBlocks()
{
    for(int i = 0; i < m_rows; i++){
        for(int j = 0; j < m_columns; j++){
            if(m_bubbles.at(i * m_columns + j)->getMarkedToDelete()){

                emit beginRemoveRows(QModelIndex(), i * m_columns + j, i * m_columns + j);
                m_bubbles.removeAt(i * m_columns + j);
                emit endRemoveRows();
                emit beginInsertRows(QModelIndex(), i * m_columns + j, i * m_columns + j);
                m_bubbles.insert(i * m_columns + j, new Bubble(nullptr, "transparent", false));
                emit endInsertRows();
                m_score += 1;
                emit scoreChanged(m_score);
            }
        }
    }
}
void Match3::moveToBottom()
{

    for(int col = 0; col < m_columns; col++){
        for(int row = m_rows - 1; row >= 0; row--){
            if(m_bubbles.at(row * m_columns + col)->getColor() == "transparent") {


                int ind = row * m_columns + col;
                int tempInd = row * m_columns + col - m_columns;
                while(tempInd >= 0){
                    if(m_bubbles.at(tempInd)->getColor() != "transparent"){

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
        while(m_bubbles.at(0 * m_columns + col)->getColor() == "transparent") {

            emit beginRemoveRows(QModelIndex(), 0 * m_columns + col, 0 * m_columns + col);
            m_bubbles.removeAt(0 * m_columns + col);
            emit endRemoveRows();
            emit beginInsertRows(QModelIndex(), 0 * m_columns + col, 0 * m_columns + col);
            m_bubbles.insert(0 * m_columns + col, new Bubble(nullptr, m_colors.at(qrand() % m_colors.size()), false));
            emit endInsertRows();
            moveToBottom();
        }
    }
}
bool Match3::checkMatches()
{

    bool matchExist = false;

    for(int i = 0; i < m_bubbles.count(); i++){
        if(m_bubbles.at(i)->getMarkedToDelete() == false){
            QColor color = m_bubbles.at(i)->getColor();
            QVector<int> onDelete;
            int temp_row = row(i);
            int temp_col = col(i);

            //printf(onDelete.isEmpty() ? "empty\n" : "something here\n");

            int connected = connectedBlocks(temp_row, temp_col, color, onDelete);
            if(onDelete.count() >= 3){
                for(int i = 0; i < onDelete.count(); i++){
                    printf("%d ", onDelete.at(i));
                }
            }

            //printf("\n");
            if(connected >= 3){
                matchExist = true;
                for(int i = 0; i < onDelete.count(); i++){
                    m_bubbles.at(onDelete.at(i))->setMarkedToDelete(true);
                    //printf("%d ", onDelete.at(i));
                }
                onDelete.clear();
                printf(" - deleted until match exist\n");
            }
        }
    }
    return matchExist;
}
void Match3::clearBubbleMarks()
{
    for(int i = 0; i < m_bubbles.count(); i++){
        m_bubbles.at(i)->setMarkedToDelete(false);
    }
}
int Match3::row(int index) const
{
    return floor(index / m_columns);
}
int Match3::col(int index) const
{
    return index % m_columns;
}

