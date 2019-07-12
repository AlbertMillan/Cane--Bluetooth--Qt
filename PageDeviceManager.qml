import QtQuick 2.12
import "."

Page {
    id: managerPage

    errorMessage: deviceHandler.error
    infoMessage: deviceHandler.info

    Button {
        id: uploadData
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: fetchData.top
        anchors.bottomMargin: Settings.fieldMargin
        width: parent.width - Settings.fieldMargin*2
        height: 50
        onClicked: {
            console.log("Upload btn");
        }

        Text {
            anchors.centerIn: parent
            font.pixelSize: Settings.tinyFontSize
            text: qsTr("UPLOAD DATA")
            color: Settings.textColor
        }
    }

    Button {
        id: fetchData
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: Settings.fieldMargin
        width: parent.width - Settings.fieldMargin*2
        height: 50
        onClicked: {
            console.log("Fetch btn")
        }

        Text {
            anchors.centerIn: parent
            font.pixelSize: Settings.tinyFontSize
            text: qsTr("FETCH DATA")
            color: Settings.textColor
        }
    }
}
