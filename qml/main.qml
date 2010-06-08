import Qt 4.7
import "elements"

Rectangle {
    id: container
    width: 200
    height: 200
    color: "#000000"

    Component {
        id: wantedItemDelegate
        Item {
            height: 40; width: container.width
            Row {
                height: 20; width: parent.width
                anchors { verticalCenter: parent.verticalCenter; horizontalCenter: parent.horizontalCenter }
                Text {
                    text: label; color: "White"
                }
                Text {
                    text: quantity; color: "White"
                }
            }
        }
    }

    ListView {
        id: wantedItemList
        model: WantedItemsModel
        delegate: wantedItemDelegate
        x: 48
        y: 49
        width: 300
        height: 100
        anchors.bottom: wantedInputBox.top
        anchors.bottomMargin: 5
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.leftMargin: 0
        anchors.rightMargin: 0
    }

    Button {
        id: addButton
        label: "Add"
        width: 60
        height: 30
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
    }

    Rectangle {
        id: wantedInputBox
        x: 1
        y: 170
        width: 135
        height: 30
        color: "#ffffff"
        radius: 5
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.right: addButton.left
        anchors.rightMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 0
        border.width: 2

        TextEdit {
            id: wantedInput
            x: 60
            y: 36
            width: wantedInputBox.width - 10
            height: 15
            text: "textEdit"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
