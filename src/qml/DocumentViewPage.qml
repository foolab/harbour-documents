import QtQuick 2.0
import Sailfish.Silica 1.0
import Documents 1.0

Page {
    allowedOrientations: Orientation.All
    backNavigation: false
    showNavigationIndicator: false

    property alias filePath: doc.filePath

    BusyIndicator {
        anchors.centerIn: parent
        size: BusyIndicatorSize.Large
        running: doc.state == Document.Loading
    }

    Flickable {
        id: flick

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: tools.top
        }

        contentWidth: doc.width * view.dpiX
        contentHeight: doc.height * view.dpiY
        boundsBehavior: Flickable.StopAtBounds

        Document {
            id: doc
        }

        PinchArea {
            anchors.fill: parent

            pinch {
                target: view
                minimumScale: 0.5
                maximumScale: 2
            }

            onPinchFinished: {
                var x = pinch.center.x * view.scale
                var y = pinch.center.y * view.scale
                var xpos = pinch.center.x - flick.contentX
                var ypos = pinch.center.y - flick.contentY
                flick.contentX = x - xpos
                flick.contentY = y - ypos
                view.zoom *= view.scale
                view.scale = 1.0
            }

            MouseArea {
                anchors.fill: parent
                onClicked: tools.shown = !tools.shown
            }
        }
    }

    DocumentView {
        id: view
        anchors.fill: flick
        document: doc.state == Document.Loaded ? doc : null
        contentX: flick.contentX
        contentY: flick.contentY
    }

    Row {
        id: tools
        property bool shown: true
        property bool _shown: shown && view.scale == 1.0 && doc.state == Document.Loaded
        visible: height > 0
        spacing: Theme.paddingMedium

        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
        }

        height: _shown ? Theme.itemSizeSmall : 0

        Behavior on height {
            NumberAnimation { duration: 200 }
        }

        IconButton {
            icon.source: "image://theme/icon-m-back"
            onClicked: pageStack.pop()
        }

        IconButton {
            icon.source: "image://svg/info.svg?"+Theme.primaryColor
        }

        IconButton {
            icon.source: "image://svg/goto.svg?"+Theme.primaryColor
        }
    }
}
