import QtQuick 2.12
import QtQuick.Window 2.3
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

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
            countText: game.steps
            textColor: "#8b4513"
        }
        NewGameButton{
            text: "New game"
            textColor: "#8b4513"
            onClicked: if(!game.newGame()) { messageDialog.open() }
        }
        Counter{
            text: "score"
            countText: game.score
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
                    }
                    /*if(!game.checkAvailableSteps()) {
                        messageDialog.open()
                    }*/
                }
            }
        }

        onAddFinished: { //show messageDialog only once if no move available
            if(!game.checkAvailableSteps()) {
                messageDialog.open()
            }
            if(game.checkMatches()){
                game.deleteBlocks();
                game.clearBubbleMarks();
                game.moveToBottom();
                game.addNewBubbles();
            }
        }

        /*onRemoveFinished: {
            //game.moveToBottom();
            //game.addNewBubbles();
            message.open()
        }*/

        onMoveFinished: {

            game.deleteBlocks();
            game.clearBubbleMarks();
            game.moveToBottom();
            game.addNewBubbles();

        }
    }

    Connections {
        target: board
        function onClicked() { onMoveFinished() }
    }

    MessageDialog {

        id: messageDialog
        text: "No valid moves =("

        onAccepted: {
            messageDialog.close()
        }
    }

    MessageDialog {

        id: message
        text: " AnimationEnded! "

        onAccepted: {
            message.close()
        }
    }

}
