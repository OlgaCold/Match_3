import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

import Match3 1.0

ApplicationWindow {

    id: root

    property int itemSize: 100
    property int marginSize: 10
    property int score: 0
    property int moves: 0

    title: qsTr("Match 3")
    visible: true
    //width: game.col * (itemSize + marginSize) + 20
    //height: game.col * (itemSize + marginSize) + 100
    width: game.col < 3 ? 3 * (itemSize + marginSize) + 20 : game.col * (itemSize + marginSize) + 20
    height: game.row * (itemSize + marginSize) + 100
    color: "#faebd7"

    minimumWidth: game.col < 3 ? 3 * (itemSize + marginSize) + 20 : game.col * (itemSize + marginSize) + 20
    minimumHeight: game.row * (itemSize + marginSize) + 100
    //maximumHeight: Screen.height - 200
    //maximumWidth: Screen.width - 200

    RowLayout{

        id: headerLayout

        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10

        Counter{
            text: "move"
            countText: moves//game.steps
            textColor: "#8b4513"
        }
        NewGameButton{
            text: "New game"
            textColor: "#8b4513"
            onClicked: {
                root.score = 0
                root.moves = 0
                if(!game.newGame()) { messageDialog.open() }
            }
        }
        Counter{
            text: "score"
            countText: score//game.score
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
        //gridWidth: game.col * cellWidth
        //gridHeight: game.row * cellHeight
        //contentWidth: game.col * cellWidth
        //contentHeight: game.row * cellHeight

        model: Match3 { id: game}

        delegate: BubbleDelegate {

            id: bubleDelegate

            color: model.color
            spacing: marginSize
            bubbleWidth: itemSize
            bubbleHeight: bubbleWidth

            state: board.isPressed && marked? "small" : "normal"

            onClick: {

                board.choosed = board.choosed + 1

                if(board.choosed === 1)
                {
                    board.first = index
                    board.isPressed = true
                    marked = true
                }

                if(board.choosed === 2)
                {
                    board.second = index
                    marked = true
                }
            }

            onRelease: {

                if(board.choosed === 2) {
                    board.itemAtIndex(board.first).marked = false;
                    board.isPressed = false
                    board.choosed = 0
                    marked = false

                    if(!game.moveHandler(board.first, board.second)){
                        board.itemAtIndex(board.first).animationStart();
                        animationStart();
                    } else {
                        root.moves++
                    }
                }
            }
        }

        onAddFinished: { //show messageDialog only once if no move available
            if(!game.checkAvailableSteps()) {
                messageDialog.open()
            }
            if(game.checkMatches()){
                root.score += game.deleteBlocks();
                game.clearBubbleMarks();
                game.moveToBottom();
                game.addNewBubbles();
            }
        }

        onMoveFinished: {

            root.score += game.deleteBlocks();
            game.clearBubbleMarks();
            game.moveToBottom();
            game.addNewBubbles();

        }
    }

    MessageDialog {

        id: messageDialog
        text: "No valid moves =("
        onAccepted: {
            messageDialog.close()
        }
    }
}
