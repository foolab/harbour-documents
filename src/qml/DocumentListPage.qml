import QtQuick 2.0
import Sailfish.Silica 1.0
import Documents 1.0
import org.nemomobile.configuration 1.0

Page {
    allowedOrientations: Orientation.All

    ConfigurationGroup {
        id: conf
        path: "/apps/harbour-documents/documentBrowser"
        property bool accurateMimeTypes: false
        property bool indexSdCards: false
        onAccurateMimeTypesChanged: refreshDocuments()
        onIndexSdCardsChanged: refreshDocuments()
    }

    Component.onCompleted: refreshDocuments()

    function refreshDocuments() {
        browser.refresh(conf.accurateMimeTypes, conf.indexSdCards)
    }

    SilicaListView {
        id: view
        anchors.fill: parent
        model: browser.model
        add: Transition { AddAnimation {} }
        remove: Transition { RemoveAnimation {} }
        header: PageHeader {
            title: qsTr("Documents")
        }

        PullDownMenu {
            id: menu
            // We delay invoking refreshDocuments() until the menu is closed
            // because it seems that clearing the list view breaks the menu animation
            property bool __refresh

            onActiveChanged: {
                if (__refresh) {
                    refreshDocuments()
                    __refresh = false
                }
            }

            MenuItem { text: qsTr("About"); onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml")) }
            MenuItem { text: qsTr("Settings"); onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml")) }
            MenuItem { text: qsTr("Refresh"); onClicked: menu.__refresh = true }
        }

        ViewPlaceholder {
            text: qsTr("No documents found. Pull down to refresh.")
            enabled: view.count == 0 && !browser.busy
        }

        BusyIndicator {
            anchors.centerIn: parent
            size: BusyIndicatorSize.Large
            running: browser.busy
        }

        delegate: ListItem {
            Label {
                anchors {
                    left: parent.left
                    leftMargin: Theme.paddingMedium
                    right: parent.right
                    rightMargin: Theme.paddingMedium
                    verticalCenter: parent.verticalCenter
                }

                truncationMode: TruncationMode.Fade
                text: name
            }
            onClicked: pageStack.push(Qt.resolvedUrl("DocumentViewPage.qml"), {filePath: path, mimeType: mime})
        }
    }

    DocumentsBrowser {
        id: browser
    }
}
