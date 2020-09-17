import QtQuick 2.12

Item {

    id: root

    property alias color: item.color
    property alias bubbleWidth: item.width
    property alias bubbleHeight: item.height
    property alias spacing: item.spacing
    property bool marked: false

    signal click
    signal release

    function animationStart() {
        return rotationAnimation.start();
    }

    width: bubbleWidth + spacing
    height: bubbleHeight + spacing
    state: "normal"

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

    SequentialAnimation {

        id: rotationAnimation

        running: false
        ParallelAnimation {
            NumberAnimation { target: item; properties: "width, height"; to:  70; duration: 100 }
            NumberAnimation { target: root; property: "spacing"; to:  40; duration: 100 }
        }
        ParallelAnimation {
            NumberAnimation { target: item; properties: "width, height"; to:  100; duration: 110 }
            NumberAnimation { target: root; property: "spacing"; to:  10; duration: 110 }
        }
        ParallelAnimation {
            NumberAnimation { target: item; properties: "width, height"; to:  80; duration: 120 }
            NumberAnimation { target: root; property: "spacing"; to:  30; duration: 120 }
        }
        ParallelAnimation {
            NumberAnimation { target: item; properties: "width, height"; to: 100; duration: 130 }
            NumberAnimation { target: root; property: "spacing"; to:  10; duration: 130 }
        }
        alwaysRunToEnd: true

    }

    MouseArea {
        id: mouseArea

        anchors.fill: parent

        pressAndHoldInterval: 0

        onPressAndHold: { root.click() }
        onReleased: { root.release() }
    }
}
