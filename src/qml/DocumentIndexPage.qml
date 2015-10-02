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
            anchors.horizontalCenter: parent.horizontalCenter
            width: Theme.itemSizeHuge
            height: Theme.itemSizeHuge * 2
            DocumentPageView {
                anchors {
                    top: parent.top
                    left: parent.left
                    right: parent.right
                    bottom: title.top
                    margins: Theme.paddingSmall
                }

                Component.onCompleted: init(doc, modelData)
            }

            Label {
                id: title
                text: qsTr("Page %1").arg(modelData + 1)
                horizontalAlignment: Text.AlignHCenter
                anchors {
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                    bottomMargin: Theme.paddingSmall
                }
            }
        }
    }

}
