import QtQuick 2.0
import Sailfish.Silica 1.0

MouseArea {
    id: self
    property alias icon: image
    readonly property bool down: pressed && containsMouse
    property bool isPressed: down || pressTimer.running

    property int itemSize: Theme.itemSizeMedium
    property int itemSizePressed: Theme.itemSizeExtraLarge

    width: itemSizePressed
    height: width

    property real enabledOpacity: 0.8
    property real disabledOpacity: 0.4

    onPressedChanged: {
        if (!isPressed)
            pressTimer.start()
    }
    onCanceled: pressTimer.stop()

    Image {
        id: image

        opacity: self.enabled ? self.enabledOpacity : self.disabledOpacity
        Behavior on opacity { FadeAnimation {} }

        readonly property int swY: (self.height - itemSize) / 2 + itemSize
        anchors.horizontalCenter: parent.horizontalCenter
        y: swY - height
        height: isPressed ? itemSizePressed: itemSize
        Behavior on height { NumberAnimation { duration: 50 } }
        width: height

        sourceSize.height: itemSize
        sourceSize.width: itemSize
    }
    Timer {
        id: pressTimer
        interval: 50
    }
}
