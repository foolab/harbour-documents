import QtQuick 2.0
import Sailfish.Silica 1.0
import Documents 1.0

Page {
    id: outlinePage

    property alias doc: outlineModel.document
    readonly property int minPadding: Theme.paddingLarge
    signal scrollTo(int page)

    DocumentOutlineModel {
        id: outlineModel
    }
    SilicaListView {
        id: outlineView
        anchors.fill: parent
        model: outlineModel
        delegate: BackgroundItem {
            id: entryItem
            height: Theme.itemSizeSmall
            width: ListView.view.width

            readonly property int levelIndentation: minPadding * (model.level + 1)

            // <-levelIndentation-> title <-minPadding-> page number <-minPadding->
            readonly property int totalPadding: levelIndentation + minPadding * 2

            // with current font size enough to display page numbers up to 99999
            readonly property int pageNrWidth: Theme.itemSizeLarge

            onClicked: outlinePage.scrollTo(model.page)

            Label {
                anchors {
                    left: parent.left
                    right: pageLabel.left
                    leftMargin: levelIndentation
                    rightMargin: minPadding
                }
                height: parent.height
                text: model.title
                verticalAlignment: Text.AlignVCenter
                elide: TruncationMode.Elide
            }
            Label {
                id: pageLabel
                height: parent.height
                width: pageNrWidth
                anchors {
                    right: parent.right
                    rightMargin: minPadding
                }
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight

                text: model.page
            }
        }
    }
}
