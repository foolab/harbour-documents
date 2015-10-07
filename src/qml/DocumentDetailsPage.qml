import QtQuick 2.0
import Sailfish.Silica 1.0
import Documents 1.0

Page {
    property Document doc

    allowedOrientations: Orientation.All

    // TODO: clean up this mess when we move to Qt 5.4:
    // https://bugreports.qt.io/browse/QTBUG-16289

    property string pageCountKey: qsTr("Page count")
    property string pageCountValue: "%1".arg(doc.pageCount)

    property string fileNameKey: qsTr("Filename")
    property string fileNameValue: Helper.fileName(doc.filePath)

    property string fileTypeKey: qsTr("Type")
    property string fileTypeValue: Helper.mimeDescription(doc.mimeType)

    ListModel {
        id: keys
        Component.onCompleted: {
            keys.append({"key": fileNameKey, "value": fileNameValue})
            keys.append({"key": fileTypeKey, "value": fileTypeValue})
            keys.append({"key": pageCountKey, "value": pageCountValue})
        }
    }

    SilicaListView {
        anchors.fill: parent
        model: keys
        header: PageHeader {
            title: qsTr("Document details")
        }

        delegate: delegate
    }

    Component {
        id: delegate
        Row {
            width: parent.width
            height: Theme.itemSizeMedium
            Label {
                width: parent.width / 2
                color: Theme.highlightColor
                text: key
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: Theme.fontSizeLarge
            }

            Label {
                width: parent.width / 2
                color: Theme.primaryColor
                text: value
                horizontalAlignment: Text.AlignLeft
                font.pixelSize: Theme.fontSizeLarge
            }
        }
    }
}
