import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {
    id: dialog
    property alias password: field.text

    Column {
        anchors.fill: parent

        DialogHeader {
            acceptText: qsTr("Unlock")
            cancelText: qsTr("Cancel")
        }

        Label {
            width: parent.width
            text: qsTr("Password to unlock document")
            font.pixelSize: Theme.fontSizeLarge
        }

        TextField {
            id: field
            width: parent.width
            focus: true
            echoMode: TextInput.Password
            EnterKey.onClicked: dialog.accept()
        }
    }
}
