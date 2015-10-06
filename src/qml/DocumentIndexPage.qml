import QtQuick 2.0
import Sailfish.Silica 1.0
import Documents 1.0

Page {
    id: page
    signal scrollTo(int page)

    property Document doc
    allowedOrientations: Orientation.All

    SilicaListView {
        clip: true

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: row.top
        }

        header: PageHeader {
                title: qsTr("Index")
        }

        model: doc.pageCount
        delegate: BackgroundItem {
            onClicked: {
                page.scrollTo(modelData)
                pageStack.pop()
            }

            height: Theme.itemSizeHuge
            anchors {
                left: parent.left
                right: parent.right
                margins: Theme.paddingLarge
            }

            DocumentPageView {
                id: thumbnail
                anchors {
                    top: parent.top
                    left: parent.left
                    bottom: parent.bottom
                    margins: Theme.paddingSmall
                }
                width: height
                Component.onCompleted: init(doc, modelData)
            }

            Label {
                text: qsTr("Page %1").arg(modelData + 1)
                verticalAlignment: Text.AlignVCenter
                anchors {
                    left: thumbnail.right
                    leftMargin: Theme.paddingLarge
                    right: parent.right
                    bottom: parent.bottom
                    top: parent.top
                }
            }
        }
    }

    Row {
        id: row
        height: Theme.itemSizeLarge
        spacing: Theme.paddingLarge

        anchors {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }

        function jump() {
            page.scrollTo(field.text - 1)
            pageStack.pop()
        }

        TextField {
            id: field
            width: Theme.itemSizeHuge * 3
            inputMethodHints: Qt.ImhDigitsOnly
            placeholderText: qsTr("Enter page number")
            validator: IntValidator {
                bottom: 1
                top: doc.pageCount
            }

            EnterKey.onClicked: row.jump()
        }

        Label {
            text: qsTr("/ %1").arg(doc.pageCount)
        }

        Button {
            text: qsTr("Go")
            onClicked: row.jump()
        }
    }
}
