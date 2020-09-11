import QtQuick 2.9

Rectangle{

    id: root

    property alias model: grid.model
    property alias delegate: grid.delegate
    property alias cellWidth: grid.cellWidth
    property alias cellHeight: grid.cellHeight
    //property alias clickAt: mouseArea.clickAt
    //property alias releasedAt: mouseArea.releasedAt
    property int first
    property int second
    property int choosed: 0
    property bool isPressed: false
    //property bool gameStarted: false
    //property string myState

    signal clicked
    signal released

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



        /*MouseArea {

            id: mouseArea

            property int clickAt
            property int releasedAt



            anchors.fill: parent

            onPressed: {
                clickAt = parent.indexAt(mouseX, mouseY)
                root.clicked();
            }

            onReleased: {
                releasedAt = parent.indexAt(mouseX, mouseY)
                root.released();
            }
        }*/
    }
}
