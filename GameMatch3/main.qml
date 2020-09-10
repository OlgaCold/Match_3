import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import Match3 1.0

ApplicationWindow {

    id: root

    property int itemSize: 100
    property int marginSize: 10

    title: qsTr("Match 3")
    visible: true
    width: game.col < 3 ? 3 * (itemSize + marginSize) + 20 : game.col * (itemSize + marginSize) + 20
    height: game.row * (itemSize + marginSize) + 100
    color: "#faebd7"

    minimumWidth: game.col < 3 ? 3 * (itemSize + marginSize) + 20 : game.col * (itemSize + marginSize) + 20
    minimumHeight: game.row * (itemSize + marginSize) + 100

    RowLayout{

        id: headerLayout

        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10

        Counter{
            text: "move"
            countText: "35"
            textColor: "#8b4513"
        }
        NewGameButton{
            text: "New game"
            textColor: "#8b4513"
            onClicked: game.newGame()
        }
        Counter{
            text: "score"
            countText: "156"
            textColor: "#8b4513"
        }
    }


    Board {

        id: board

        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 40
        anchors.horizontalCenter: parent.horizontalCenter

        cellWidth: itemSize + marginSize
        cellHeight: cellWidth
        width: game.col * cellWidth
        height: game.row * cellHeight
        myState: choosed == 2 ?  "small" : "normal"

        model: Match3 { id: game}
        delegate: BubbleDelegate {

            id: bubleDelegate

            color: model.color
            spacing: marginSize
            bubbleWidth: itemSize
            bubbleHeight: bubbleWidth
            state: isPressed ? board.myState : "normal"//board.myState//
            onClick: {

                //state = "small"
                isPressed = true
                board.choosed = board.choosed + 1

                if(board.choosed === 1)
                {
                    board.first = index

                }

                if(board.choosed === 2)
                {
                    board.second = index
                    game.move(board.first, board.second)
                }

            }
            onRelease: {

                if(board.choosed === 2) {
                    isPressed = false
                    board.choosed = 0
                }
            }

        }
    }
}
