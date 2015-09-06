import QtQuick 2.0
import Poppler 1.0

Rectangle {
width: 400; height: 600

    Flickable {
        id: flick

// TODO: doc.document.Width seems incorrect
        anchors.fill: parent
        contentWidth: doc.documentWidth
        contentHeight: doc.documentHeight
        boundsBehavior: Flickable.StopAtBounds

        onContentXChanged: console.log("contentX = " + contentX)
        PopplerDocument {
            id: doc
            filePath: "/home/mohammed/slides.pdf"
//            filePath: "/home/mohammed/Wada3.pdf"
        }

        focus: true
        Keys.onRightPressed: {
            var x = flick.contentX + 10
            if (x > flick.contentWidth) {
                x = flick.contentWidth
            }

            flick.contentX = x
        }

        Keys.onLeftPressed: {
            var x = flick.contentX - 10
            if (x < 0) {
                x = 0
            }

            flick.contentX = x
        }

        Keys.onDownPressed: {
            var y = flick.contentY + 10
            if (y > flick.contentHeight) {
                x = flick.contentHeight
            }

            flick.contentY = y
        }

        Keys.onUpPressed: {
            var y = flick.contentY - 10
            if (y < 0) {
                y = 0
            }

            flick.contentY = y
        }

    }

    DocumentView {
        anchors.fill: flick
        document: doc
        contentX: flick.contentX
        contentY: flick.contentY
    }
}
