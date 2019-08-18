import QtQuick 2.0
import Options 1.0
import Settings 1.0

Page {

    ListView {
        id: users
        implicitHeight: parent.height
        anchors.margins: Settings.fieldMargin
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: Settings.fieldMargin
        clip: true

        model: OptionsModel {
            list: optionsHandler
        }

        delegate: Rectangle {
            id: userBox
            height: Settings.fieldHeight*1.5
            width: parent.width
            color: Settings.buttonColor
            radius: Settings.buttonRadius

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    // 1. RETRIEVE DATA FROM CLICKED ELEMENT
                    optionsHandler.setOptions(index);


                    // 2. GO TO PAGECONNECT.QML
                    app.showPage("PageConnect.qml")
                }
            }

            Text {
                id: user
                font.pixelSize: Settings.mediumFontSize
                text: model.username
                anchors.centerIn: parent
                color: Settings.textColor
            }
        }
    }

    Rectangle {
        id: addBtn
        height: Settings.fieldHeight*1.5
        anchors.bottomMargin: Settings.fieldMargin
        anchors.bottom: parent.bottom
        width: parent.width - Settings.fieldMargin*2
        anchors.horizontalCenter: parent.horizontalCenter
        color: "red"
        radius: Settings.buttonRadius

        MouseArea {
            anchors.fill: parent
            onClicked: {
                app.showPage("PageRegister.qml")
            }
        }

        Text {
            font.pixelSize: Settings.mediumFontSize
            text: "+"
            anchors.centerIn: parent
            color: Settings.textColor
        }
    }

}
