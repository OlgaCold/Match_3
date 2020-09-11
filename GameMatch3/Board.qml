import QtQuick 2.9

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

    signal clicked
    signal released

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

        displaced: Transition {
            NumberAnimation { properties: "x,y,"; easing.type: Easing.OutQuad }
        }
        move: Transition {
            NumberAnimation { properties: "x,y"; easing.type: Easing.OutQuad }
        }
    }
}
