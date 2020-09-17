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
        return tremblingAnimation.start();
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
            NumberAnimation { properties: "width, height, spacing"; duration: 100;
                easing.type: Easing.Linear }
        }
    }

    SequentialAnimation {
        id: tremblingAnimation

        running: false
        ParallelAnimation {
            NumberAnimation { target: item; properties: "width, height"; to:  100; duration: 500;
                easing.amplitude: 2.5; easing.period: 1.5; easing.type: Easing.OutBounce}
            NumberAnimation { target: root; property: "spacing"; to:  10; duration: 500;
                easing.amplitude: 2.5; easing.period: 1.5; easing.type: Easing.OutBounce }
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
