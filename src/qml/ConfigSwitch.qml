import QtQuick 2.0
import Sailfish.Silica 1.0
import org.nemomobile.configuration 1.0

TextSwitch {
    property alias key: conf.key
    property alias defaultValue: conf.defaultValue
    property alias value: conf.value

    checked: conf.value == true
    onCheckedChanged: conf.value = checked

    ConfigurationValue {
        id: conf
    }

}
