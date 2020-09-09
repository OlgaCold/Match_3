import QtQuick 2.9

Item {

    id: root

    property alias color: item.color
    property alias bubbleWidth: item.width
    property alias bubbleHeight: item.height
    //property alias clickAt: mouseArea.clickAt
    //property alias releasedAt: mouseArea.releasedAt
    property int spacing

    width: bubbleWidth + spacing
    height: bubbleHeight + spacing

    Bubble {

        id: item;
        anchors.centerIn: parent
    }
}
