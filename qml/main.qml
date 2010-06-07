import Qt 4.7

Rectangle {
    id: rectangle1
    width: 200
    height: 200
    color: "#000000"

    ListView {
        id: wantedItemList
        x: 48
        y: 49
        width: 300
        height: 300
        anchors.bottom: wantedInput.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottomMargin: 5
        anchors.topMargin: 0
        anchors.leftMargin: 0
        anchors.rightMargin: 0
    }

    Rectangle {
        id: addButton
        x: 13
        y: 87
        width: 60
        height: 30
        color: "#3e3535"
        radius: 8
        border.width: 2
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0

        Text {
            id: addButtonLabel
            x: 10
            y: 40
            width: addButton.width - 20
            text: "Add"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }

        MouseArea {
            id: addButtonMouseArea
            x: 0
            y: 0
            width: 100
            height: 100
            anchors.bottomMargin: 0
            anchors.topMargin: 0
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.fill: parent
        }
    }

    TextEdit {
        id: wantedInput
        x: 60
        y: 36
        width: 80
        height: addButton.height
        text: "textEdit"
        anchors.right: addButton.left
        anchors.rightMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
    }
}
