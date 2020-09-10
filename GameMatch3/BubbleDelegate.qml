import QtQuick 2.9

Item {

    id: root

    property alias color: item.color
    property alias bubbleWidth: item.width
    property alias bubbleHeight: item.height
    property bool isPressed
    //property alias first: mouseArea.first
    //property alias second: mouseArea.second
    state: "normal"
    //property alias releasedAt: mouseArea.releasedAt
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

            /*choosed = choosed + 1
            //root.state = "small"
            clickAt = index;

            if(choosed === 1)
            {
                first = clickAt
            }

            if(choosed === 2)
            {
                second = clickAt
                choosed = 0
                root.choosedTwoItems()
            }
            console.log(choosed)
            console.log(clickAt)*/
        }
        onReleased: { root.release() }
    }
}
