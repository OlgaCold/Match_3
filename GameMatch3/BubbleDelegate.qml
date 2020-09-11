import QtQuick 2.9

Item {

    id: root

    property alias color: item.color
    property alias bubbleWidth: item.width
    property alias bubbleHeight: item.height
    property bool marked: false
    state: "normal"
    property alias spacing: item.spacing
    signal click
    signal release
    //signal choosedTwoItems

    width: bubbleWidth + spacing
    height: bubbleHeight + spacing

    Bubble {

        id: item;
        property int spacing
        anchors.centerIn: parent

    }

    states: [
        State {
            name: "small"
            PropertyChanges { target: item; width: 80; height: 80; spacing: 30}
        },
        State {
            name: "normal"
            PropertyChanges { target: item; width: 100; height: 100; spacing: 10}
        }
    ]
    transitions: Transition {
        from: "normal"; to: "small"
        reversible: true
        ParallelAnimation {
            NumberAnimation { properties: "width, height, spacing"; duration: 100; easing.type: Easing.Linear }
        }
    }

    MouseArea {
        id: mouseArea

        //property int clickAt
        //property int first
        //property int second

        anchors.fill: parent

        pressAndHoldInterval: 0

        onPressAndHold: {

            root.click()
        }
        onReleased: { root.release() }
    }
}
