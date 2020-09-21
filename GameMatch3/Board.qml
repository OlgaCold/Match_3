import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle{

    id: root

    property alias model: grid.model
    property alias delegate: grid.delegate
    property alias cellWidth: grid.cellWidth
    property alias cellHeight: grid.cellHeight
    property int first
    property int second
    property int choosed: 0
    property bool isPressed: false

    signal addFinished
    signal moveFinished

    function itemAtIndex(index) {
        return grid.itemAtIndex(index);
    }

    function animationRunning() {
        if(addAnimation.running || moveAnimation.running || moveDisplacedAnimation.running){
            return true
        } else {
            return false
        }
    }

    color: "transparent"
    border.color: "#8b4513"
    border.width: 2

    GridView {

        id: grid

        anchors.fill: parent
        interactive: false

        currentIndex: -1
        clip: true

        move: Transition {
            id: moveAnimation
            onRunningChanged: {
                if (!moveAnimation.running) {
                    root.moveFinished()
                }
            }
            NumberAnimation { properties: "x, y"; duration: 500; easing.type: Easing.OutQuad }
        }

        moveDisplaced:Transition {
            id: moveDisplacedAnimation
            onRunningChanged: {
                if (!moveDisplacedAnimation.running) {
                    root.moveFinished()
                }
            }
            NumberAnimation { properties: "x, y"; duration: 500; easing.type: Easing.OutQuad }
        }

        remove:Transition {
            id: removeAnimation
            NumberAnimation { property: "opacity"; to: 0; duration: 200 }
        }

        add: Transition {
            id: addAnimation
            onRunningChanged: {
                if (!addAnimation.running) {
                    root.addFinished()
                }
            }
            NumberAnimation { properties: "y"; from: -100; duration: 500; easing.type: Easing.OutQuad }
        }
    }
}
