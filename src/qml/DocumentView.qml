import QtQuick 2.0
import Sailfish.Silica 1.0
import Documents 1.0

Page {
    allowedOrientations: Orientation.All
    backNavigation: false
    showNavigationIndicator: false

    property alias filePath: doc.filePath

    Flickable {
        id: flick

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: tools.top
        }

        contentWidth: doc.width
        contentHeight: doc.height
        boundsBehavior: Flickable.StopAtBounds

        Document {
            id: doc
        }

        MouseArea {
            anchors.fill: parent
            onClicked: tools.shown = !tools.shown
        }
    }

    DocumentView {
        id: view
        anchors.fill: flick
        document: doc
        contentX: flick.contentX
        contentY: flick.contentY
    }

    Row {
        id: tools
        property bool shown: true

        visible: height > 0

        anchors {
            right: parent.right
            left: parent.left
            bottom: parent.bottom
        }

        height: shown ? Theme.itemSizeSmall : 0

        Behavior on height {
            NumberAnimation { duration: 200 }
        }

        IconButton {
            icon.source: "image://theme/icon-m-back"
            onClicked: pageStack.pop()
        }

        Button {
            text: "+"
            onClicked: view.zoom += 0.1
        }

        Button {
            text: "-"
            onClicked: view.zoom -= 0.1
        }
    }
}
