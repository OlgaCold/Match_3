import QtQuick 2.12

Rectangle {

    id: root

    radius: height/2
    border.color: Qt.darker(color)

    signal pressAndHold
    signal released

    MouseArea {
        id: mouseArea

        anchors.fill: parent
        pressAndHoldInterval: 0

        onPressAndHold: { root.pressAndHold() }
        onReleased: { root.released() }
    }
}
