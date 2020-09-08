import QtQuick 2.9

Rectangle{

    id: root

    property alias model: grid.model
    property alias delegate: grid.delegate
    property alias cellWidth: grid.cellWidth
    property alias cellHeight: grid.cellHeight

    color: "transparent"
    border.color: "#8b4513"
    border.width: 2

    GridView {

        id: grid
        anchors.fill: parent
        interactive: false
    }
}
