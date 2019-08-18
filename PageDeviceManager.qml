import QtQuick 2.12
import Settings 1.0
import "."

Page {
    id: managerPage

    errorMessage: deviceHandler.error
    infoMessage: deviceHandler.info


    Button {
        id: startbtn
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: stopbtn.top
        anchors.bottomMargin: Settings.fieldMargin
        width: parent.width - Settings.fieldMargin*2
        height: Settings.fieldHeight
        onClicked: {

            deviceHandler.setUnitMeasurement(optionsHandler.unit, optionsHandler.time);
            deviceHandler.startMeasurement();
            deviceFinder.test()
        }

        Text {
            anchors.centerIn: parent
            font.pixelSize: Settings.tinyFontSize
            text: qsTr("START")
            color: Settings.textColor
        }
    }

    Button {
        id: stopbtn
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: Settings.fieldMargin
        width: parent.width - Settings.fieldMargin*2
        height: Settings.fieldHeight
        onClicked: {
            deviceHandler.stopMeasurement();
        }

        Text {
            anchors.centerIn: parent
            font.pixelSize: Settings.tinyFontSize
            text: qsTr("STOP")
            color: Settings.textColor
        }
    }
}
