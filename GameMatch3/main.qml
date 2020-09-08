import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

ApplicationWindow {

    id: root

    property int maxSize

    title: qsTr("Match 3")
    visible: true
    width: 4 * 110 + 20
    height: 3 * 110 + 100
    color:"#faebd7"

    minimumWidth: 4 * 110 + 20
    minimumHeight: 3 * 110 + 100

    RowLayout{

        id: headerLayout

        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10

        Counter{
            text: "move"
            textColor: "#8b4513"
        }
        NewGameButton{
            text: "New game"
            textColor: "#8b4513"
            //onClicked:
        }
        Counter{
            text: "score"
            textColor: "#8b4513"
        }
    }

    Board {

        id: board

        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 40
        anchors.horizontalCenter: parent.horizontalCenter

        width: 4 * 110
        height: 3 * 110

        cellWidth: 110
        cellHeight: cellWidth

        model: 4*3
        delegate: BubbleDelegate {

            id: bubleDelegate
            color: "#cd853f"
        }
    }
}
