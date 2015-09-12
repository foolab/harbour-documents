import QtQuick 2.0
import Sailfish.Silica 1.0
import Documents 1.0

Page {
    backNavigation: false
    showNavigationIndicator: false

    property alias filePath: doc.filePath

    Flickable {
        id: flick

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: row.top
        }

        contentWidth: doc.documentWidth
        contentHeight: doc.documentHeight
        boundsBehavior: Flickable.StopAtBounds

        Document {
            id: doc
        }
    }

    DocumentView {
        anchors.fill: flick
        document: doc
        contentX: flick.contentX
        contentY: flick.contentY
    }

    Row {
        id: row
        anchors {
            right: parent.right
            left: parent.left
            bottom: parent.bottom
        }

        height: Theme.itemSizeSmall

        IconButton {
            icon.source: "image://theme/icon-m-back"
            onClicked: pageStack.pop()
        }

        Button {
            text: "+"
            onClicked: doc.zoom += 0.1
        }

        Button {
            text: "-"
            onClicked: doc.zoom -= 0.1
        }
    }
}
