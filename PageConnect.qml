import QtQuick 2.12
import Settings 1.0
import "."

Page {
    errorMessage: deviceFinder.error
    infoMessage: deviceFinder.info

    Rectangle {
        id: viewContainer
        anchors.top: parent.top
        anchors.bottom: searchButton.top
        anchors.topMargin: Settings.fieldMargin + messageHeight
        anchors.bottomMargin: Settings.fieldMargin
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - Settings.fieldMargin*2
        color: Settings.viewColor
        radius: Settings.buttonRadius

        Text {
            id: title
            width: parent.width
            height: Settings.fieldHeight
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: Settings.textColor
            font.pixelSize: Settings.mediumFontSize
            text: qsTr("FOUND DEVICES")

            BottomLine {
                height: 1
                width: parent.width
                color: "#898989"
            }
        }

        ListView {
            id: devices
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.top: title.bottom
            model: deviceFinder.devices
            clip: true

            delegate: Rectangle {
                id: box
                height: Settings.fieldHeight*1.2
                width: parent.width
                color: index % 2 === 0 ? Settings.delegate1Color : Settings.delegate2Color
                property bool selected: false

                MouseArea {
                    anchors.fill: parent
                    onClicked: {

                        if( !deviceFinder.scanning ) {

                            if( !deviceFinder.deviceLimitReached && !box.selected ) {
                                deviceFinder.storeAddress(modelData.deviceAddress)
                                box.color = "#33ccff"
                                box.selected = true
                            }
                            else if(box.selected) {
                                deviceFinder.removeAddress(modelData.deviceAddress)
                                box.color = index % 2 === 0 ? Settings.delegate1Color : Settings.delegate2Color
                                box.selected = false
                            }

                        }
                    }
                }

                Text {
                    id: device
                    font.pixelSize: Settings.smallFontSize
                    text: modelData.deviceName
                    anchors.top: parent.top
                    anchors.topMargin: parent.height * 0.1
                    anchors.leftMargin: parent.height * 0.1
                    anchors.left: parent.left
                    color: Settings.textColor
                }

                Text {
                    id: deviceAddress
                    font.pixelSize: Settings.smallFontSize
                    text: modelData.deviceAddress
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: parent.height * 0.1
                    anchors.rightMargin: parent.height * 0.1
                    anchors.right: parent.right
                    color: Qt.darker(Settings.textColor)
                }
            }
        }
    }

    Button {
        id: searchButton
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: connectButton.top
        anchors.bottomMargin: Settings.fieldMargin
        width: parent.width - Settings.fieldMargin*2
        height: Settings.fieldHeight
        enabled: !deviceFinder.scanning
        onClicked: deviceFinder.startSearch()

        Text {
            anchors.centerIn: parent
            font.pixelSize: Settings.tinyFontSize
            text: qsTr("START SEARCH")
            color: searchButton.enabled ? Settings.textColor : Settings.disabledTextColor
        }
    }

    Button {
        id: connectButton
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: Settings.fieldMargin
        width: parent.width - Settings.fieldMargin*2
        height: Settings.fieldHeight
        enabled: deviceFinder.selected === 0
        onClicked: {
//            Connect to Devices
            console.log("CONNECTING")
            deviceFinder.connectToService()

            app.showPage("PageDeviceManager.qml")
        }

        Text {
            anchors.centerIn: parent
            font.pixelSize: Settings.tinyFontSize
            text: qsTr("CONTINUAR") + "("+ deviceFinder.selected +")"
            color: connectButton.enabled ? Settings.textColor : Settings.disabledTextColor
        }
    }
}
