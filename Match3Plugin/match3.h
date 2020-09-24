#pragma once

#include <QAbstractListModel>
#include <QString>

struct Bubble;

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

    Q_INVOKABLE bool newGame(); //return false if no move available
    Q_INVOKABLE bool moveHandler(int clicked, int released); //return true if connected > 2
    Q_INVOKABLE bool checkAvailableSteps(); //brute force algorithm, return true if step available
    Q_INVOKABLE bool checkMatches(); //return true if match exist and mark bubbles to delete
    Q_INVOKABLE int deleteHandler();

private:
    int m_rows;
    int m_columns;
    QList<Bubble*> m_bubbles;
    QList<QColor> m_colors;

    int deleteBlocks(); //remove and insert transparent blocks by marks
    void moveToBottom(); //move transparent up
    void addNewBubbles(); //delete transparent and insert random color bubble
    void clearBubbleMarks(); // set all Bubble m_markedToDelete to false
    bool configFromJson(const QString path);
    void createStandartJson(const QString path);
    bool checkMove(int from, int to) const; // return true, if gamer move is valid
    int connectedBlocks(int row, int col, QColor color, QVector<int> &toDelete, QList<Bubble*> &board); //for copy of the Board
    void fillField();
    QList<Bubble*> copyBoard();
    void deleteBoard(QList<Bubble*> &board);
    void move(int clicked, int released);
    int row(int index) const;
    int col(int index) const;
    int getIndex(int row, int col) const;
};
