import QtQuick 2.12
import QtQuick.Layouts 1.3

Rectangle {

    id: root

    property alias text: label.text
    property alias countText: countLabel.text
    property alias textColor: label.color

    width: 100
    height: 70
    color: "#00000000"

    ColumnLayout{

        id: textLayout
        spacing: 6
        anchors.centerIn: parent

        Text {
            id: label

            font.pixelSize: 26
            color: "#b8860b"
            Layout.alignment:  Qt.AlignHCenter
        }
        Text {
            id: countLabel
            text: "34"
            font.pixelSize: 26
            color: "#b8860b"
            Layout.alignment:  Qt.AlignHCenter
        }
    }

}
