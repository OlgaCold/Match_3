import QtQuick 2.9

Rectangle {

    id: root

    property alias text: label.text
    property alias textColor: label.color

    signal clicked

    width: 150
    height: 70
    radius: 30
    color: "#faf0e6"
    border.color: "#8b4513"

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: root.clicked()
    }

    Text {
        id: label

        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        font.pixelSize: 26
        color: "#b8860b"
    }
}
