import QtQuick 2.0
import Poppler 1.0

Rectangle {
width: 400; height: 600

    Flickable {
        id: flick

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

    }

    DocumentView {
        anchors.fill: flick
        document: doc
        contentX: flick.contentX
        contentY: flick.contentY
    }
}
