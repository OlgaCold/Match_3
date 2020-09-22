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
    width:  game.col > 13 ? 13 * (itemSize + marginSize) + 20 : game.col * (itemSize + marginSize) + 20
    height: game.row > 8 ? 8 * (itemSize + marginSize) + 120 : game.row * (itemSize + marginSize) + 120
    color: "#faebd7"

    minimumWidth: 3 * (itemSize + marginSize) + 20
    minimumHeight: 3 * (itemSize + marginSize) + 120

    ColumnLayout {

        anchors.fill: parent

        RowLayout{

            id: headerLayout

            Layout.margins: 10
            Layout.alignment: Qt.AlignHCenter
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

                    if(!board.animationRunning()){
                        root.score = 0
                        root.moves = 0
                        board.choosed = 0
                        if(!game.newGame()) { messageDialog.open() }
                    }
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

            width: root.width - 20
            height: root.height - 120

            clip: true
            interactive: false

            Layout.alignment: Qt.AlignCenter
            Layout.margins: 10

            contentWidth: board.width
            contentHeight: board.height

            focus: true
            Keys.onUpPressed: vbar.decrease()
            Keys.onDownPressed: vbar.increase()
            Keys.onLeftPressed: hbar.decrease()
            Keys.onRightPressed: hbar.increase()

            ScrollBar.vertical: ScrollBar {
                id: vbar;
                policy: ScrollBar.AlwaysOn
            }
            ScrollBar.horizontal: ScrollBar {
                id: hbar;
                policy: ScrollBar.AlwaysOn;
            }


            Board {

                id: board

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

                        if(!board.animationRunning()) {

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
                    }

                    onRelease: {

                        if(board.itemAtIndex(board.first) === null){
                            board.choosed = 0
                            board.first = -1
                        } else {
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
                }

                onAddFinished: {

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
}
