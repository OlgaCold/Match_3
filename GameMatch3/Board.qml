import QtQuick 2.12

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

    signal finished

    function itemAtIndex(index) {
        return grid.itemAtIndex(index);
    }

    color: "transparent"
    border.color: "#8b4513"
    border.width: 2

    GridView {

        id: grid
        anchors.fill: parent
        interactive: false

        currentIndex: -1

        move: Transition {
            NumberAnimation { properties: "x, y"; duration: 500; easing.type: Easing.OutQuad }
        }

        moveDisplaced:Transition {
            SequentialAnimation {
                NumberAnimation { properties: "x, y"; duration: 1000; easing.type: Easing.OutQuad }//1000
            }
        }

        remove: Transition {
            SequentialAnimation {
                NumberAnimation { property: "opacity"; to: 0; duration: 200 }
            }
        }

        add: Transition {
            SequentialAnimation {
                id: addAnimation
                onFinished: { root.finished() }
                NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 500 }
                NumberAnimation { properties: "y"; duration: 500; easing.type: Easing.OutQuad }
            }
        }
    }
}
