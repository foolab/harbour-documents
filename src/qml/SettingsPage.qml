import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            width: parent.width

            PageHeader {
                title: qsTr("Settings")
            }

            ConfigSwitch {
                key: "/apps/harbour-documents/documentBrowser/accurateMimeTypes"
                text: qsTr("Slower but more accurate file type detection")
            }

            ConfigSwitch {
                key: "/apps/harbour-documents/documentBrowser/indexSdCards"
                text: qsTr("Check MMC cards for documents")
            }
        }
    }
}
