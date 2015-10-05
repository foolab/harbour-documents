import QtQuick 2.0
import Sailfish.Silica 1.0
import Documents 1.0

Page {
    id: page
    signal scrollTo(int page)

    property Document doc
    allowedOrientations: Orientation.All

    SilicaListView {
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: parent.bottom // TODO:
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

}
