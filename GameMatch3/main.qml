import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
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
    width: game.col * (itemSize + marginSize) + 20
    height: game.row * (itemSize + marginSize) + 100
    color: "#faebd7"

    minimumWidth: 3 * (itemSize + marginSize) + 20
    minimumHeight: 3 * (itemSize + marginSize) + 100

    RowLayout{

        id: headerLayout

        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10

        Counter{
            text: "move"
            countText: moves
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
            countText: score
            textColor: "#8b4513"
        }
    }


    Flickable {

        id: flickable

        width: root.width > Screen.width - 200 ? Screen.width - 220 : root.width - 20
        height: root.height > Screen.height - 200 ? Screen.height - 300 : root.height - 100

        clip: true
        interactive: false

        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 40
        anchors.horizontalCenter: parent.horizontalCenter

        contentWidth: board.width
        contentHeight: board.height

        focus: true
        Keys.onUpPressed: vbar.decrease()
        Keys.onDownPressed: vbar.increase()
        Keys.onLeftPressed: hbar.decrease()
        Keys.onRightPressed: hbar.increase()

        ScrollBar.vertical: ScrollBar { id: vbar}
        ScrollBar.horizontal: ScrollBar { id: hbar}

        Board {

            id: board

            anchors.centerIn: parent

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
}
