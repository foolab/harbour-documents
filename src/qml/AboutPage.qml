import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent

        Column {
            id: column
            spacing: Theme.paddingLarge

            anchors {
                fill: parent
                margins: Theme.paddingLarge
            }

            Image {
                anchors.horizontalCenter: parent.horizontalCenter
                width: Theme.itemSizeHuge
                height: width
                source: "image://svg/logo.svg"
            }

            Label {
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: Theme.fontSizeExtraLarge
                text: qsTr("Document Viewer")
            }

            Label {
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: Theme.fontSizeMedium
                text: qsTr("Copyright (c) 2015 Mohammed Sameer")
            }

            BackgroundItem {
                width: parent.width
                height: license.height
                Label {
                    id: license
                    wrapMode: Text.Wrap
                    width: parent.width
                    font.pixelSize: Theme.fontSizeSmall
                    color: parent.pressed ? Theme.secondaryColor : Theme.primaryColor
                    text: qsTr("This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.\n
    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.\n
    You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.")
                }
            }
        }
    }
}
