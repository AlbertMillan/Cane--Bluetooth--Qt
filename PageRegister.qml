import QtQuick 2.12
import QtQuick.Layouts 1.12
import Settings 1.0
import "."

Page {

    InputText {
        id: username
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: Settings.fieldMargin
        width: parent.width - Settings.fieldMargin*2
        height: Settings.fieldHeight
        innerText: qsTr("Usuario")
        maxLength: 30
        reference: 0
    }

    InputText {
        id: weight
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: username.bottom
        anchors.topMargin: Settings.fieldMargin
        width: parent.width - Settings.fieldMargin*2
        height: Settings.fieldHeight
        innerText: qsTr("Peso(kg)")
        maxLength: 3
        validator: IntValidator { bottom: 1; top: 400;}
        reference: 1
    }

    RowLayout {
        id: measurement
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: weight.bottom
        anchors.topMargin: Settings.fieldMargin
        spacing: parent.width/4
        height: 220

        ButtonRadio {
            id: pasos
            btnText: qsTr("Pasos")
            btnChecked: true
        }
        ButtonRadio {
            btnText: qsTr("Tiempo")
            btnChecked: false
        }
    }

    InputText {
        id: unitMeasurement
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: measurement.bottom
        anchors.topMargin: Settings.fieldMargin
        width: parent.width - Settings.fieldMargin*2
        height: Settings.fieldHeight
        innerText: pasos.checked ? qsTr("Numero de Pasos") : qsTr("Tiempo(minutos)")
        validator: IntValidator { bottom: 0; top: 1440;}
        reference: 2
    }

    RowLayout {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: unitMeasurement.bottom
        anchors.topMargin: Settings.fieldMargin
        spacing: parent.width/4

        ButtonRadio {
            id: pole
            btnText: qsTr("Unilateral")
            btnChecked: true
        }
        ButtonRadio {
            btnText: qsTr("Bilateral")
            btnChecked: false
        }
    }


    Button {
        id: searchButton
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: Settings.fieldMargin
        width: parent.width - Settings.fieldMargin*2
        height: Settings.fieldHeight
        onClicked: {
            // 1. VERIFY INPUT
            var valid = true
            if(!weight.isValid()) {
                weight.border.color = "red"
                valid = false
                // SEND NOTIFICATION UP PHONE...
            }

            if(!unitMeasurement.isValid()) {
                unitMeasurement.border.color = "red"
                valid = false
            }

            if(username.getInnerText().length === 0) {
                username.border.color = "red"
                valid = false
            }

            if(valid) {

                var nPoles = (pole.checked ? 1 : 2);

                // 2. CREATE USER
                optionsHandler.addUser(username.getInnerText(),
                                       weight.getInnerText(),
                                       (pasos.checked ? 1 : 2),
                                       unitMeasurement.getInnerText(),
                                       nPoles
                                      )

                deviceFinder.setMaxDevices(nPoles)

                // 3. MOVE TO USER MENU
                app.showPage("PageUsers.qml")
            }
            else {
                console.log("Invalid Input")
            }
        }

        Text {
            anchors.centerIn: parent
            font.pixelSize: Settings.tinyFontSize
            text: qsTr("REGISTRAR USUARIO")
            color: searchButton.enabled ? Settings.textColor : Settings.disabledTextColor
        }
    }

}
