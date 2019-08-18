import QtQuick 2.12
import QtQuick.Controls 2.12
import Settings 1.0

Rectangle {
    id: outerBox
    color: Settings.buttonColor
    radius: Settings.buttonRadius
    border.color: "white"


    property alias innerText: textField.placeholderText
    property alias maxLength: textField.maximumLength
    property alias validator: textField.validator
    property int   reference: 0

    TextField {
        id: textField
        background: parent
        anchors.fill: parent
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 15
        anchors.rightMargin: 15
        color: "white"
        font.pointSize: Settings.microFontSize
        placeholderTextColor: Settings.placeholderColor
        onFocusChanged: {
            outerBox.border.color = "white"
        }

        onTextChanged: {
//            console.log(reference, textField.text);
//            optionsHandler.setText(reference, textField.text);
        }
    }

    function getInnerText() {
        return textField.text;
    }

    function isValid() {
        return textField.acceptableInput;
    }
}
