import Qt 4.7

Rectangle {
    id: buttonRect
    width: 640
    height: 480
    color: "#6b6b6b"
    radius: 8
    border.width: 2

    MouseArea {
        id: buttonMouseArea
        x: 191
        y: 233
        width: 100
        height: 100
        anchors.bottomMargin: 0
        anchors.topMargin: 0
        anchors.leftMargin: 0
        anchors.rightMargin: 0
        anchors.fill: parent
    }

    Text {
        id: buttonLabel
        x: 111
        y: 113
        width: buttonRect.width-10
        height: buttonRect.height-10
        text: "text"
        horizontalAlignment: "AlignHCenter"
        verticalAlignment: "AlignVCenter"
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: buttonRect.height * 0.6
        anchors.verticalCenter: parent.verticalCenter
    }
}
