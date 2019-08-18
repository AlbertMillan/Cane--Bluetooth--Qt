import QtQuick 2.0
import QtQuick.Controls 2.12



RadioButton {
    id: control
//    font.pointSize: Settings.smallFontSize

    indicator: Rectangle {
        id: radio
        anchors.verticalCenter: parent.verticalCenter
        implicitWidth: Settings.radioBtnHeight
        implicitHeight: Settings.radioBtnHeight
        radius: Settings.radioBtnHeight
        color: Settings.buttonColor
        border.color: "white"
        border.width: 1
        Rectangle {
            anchors.fill: parent
            visible: control.checked
            color: "white"
            radius: Settings.radioBtnHeight
            anchors.margins: Settings.radioBtnHeight/9
        }
    }

    contentItem: Text {
        id: radioText
        text: control.text
        font.pointSize: Settings.smallFontSize
        anchors.left: radio.right
        anchors.leftMargin:12
        color: "white"
        verticalAlignment: Text.AlignVCenter

    }


    property alias btnText: control.text
    property alias btnChecked: control.checked
}
