import QtQuick 2.0
import Sailfish.Silica 1.0
import Documents 1.0

Page {
    allowedOrientations: Orientation.All

    SilicaListView {
        id: view
        anchors.fill: parent
        model: browser.model

        header: PageHeader {
            title: qsTr("Documents")
        }

        PullDownMenu {
            MenuItem { text: qsTr("About") }
            MenuItem { text: qsTr("Settings") }
            MenuItem { text: qsTr("Refresh") }
        }

        ViewPlaceholder {
            text: qsTr("No documents found. Pull down to refresh.")
            enabled: view.count == 0
        }

        delegate: BackgroundItem {
            Label {
                width: parent.width
                anchors.verticalCenter: parent.verticalCenter
                truncationMode: TruncationMode.Fade
                text: name
            }
            onClicked: pageStack.push(Qt.resolvedUrl("DocumentView.qml"), {filePath: path})
        }
    }

    DocumentsBrowser {
        id: browser
    }
}
