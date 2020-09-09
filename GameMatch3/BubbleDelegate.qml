import QtQuick 2.9

Item {

    id: root

    property alias color: item.color
    property alias bubbleWidth: item.width
    property alias bubbleHeight: item.height
    property int spacing

    width: bubbleWidth + spacing
    height: bubbleHeight + spacing

    //property alias oldP: mouseArea.oldPosition
    //property alias text: item.text
    //property alias isVoid: item.isVoid

    //signal clicked

    Bubble {

        id: item;
        anchors.centerIn: parent
    }

    /*MouseArea {

        id: mouseArea

        property int oldPosition
        property bool pressed

        anchors.fill: parent

        onPressed: {

            oldPosition = index
            root.clicked();
        }
    }*/
}
