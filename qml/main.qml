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
                    text: label + " x " + quantity; color: "White"
                    horizontalAlignment: "AlignHCenter"
                    verticalAlignment: "AlignVCenter"
                }
            }
        }
    }

    ListView {
        id: wantedItemList
        model: WantedItemsModel
        delegate: wantedItemDelegate

        anchors { bottom: wantedInputBox.top; bottomMargin: 5; right: parent.right; left: parent.left; top: parent.top }
    }

    Button {
        id: addButton
        label: "Add"
        width: 60; height: 30

        anchors { right: parent.right; bottom: parent.bottom }

        onClicked: { WantedItemsModel.addItem(wantedInput.text) }
    }

    Rectangle {
        id: wantedInputBox
        color: "#ffffff"
        radius: 5
        border.width: 2
        height: 30

        anchors { bottom: parent.bottom; right: addButton.left; rightMargin: 5; left: parent.left }

        TextEdit {
            id: wantedInput

            width: wantedInputBox.width - 10; height: 15
            text: ""

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
