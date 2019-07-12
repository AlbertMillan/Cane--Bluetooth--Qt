import QtQuick 2.12
import Settings 1.0
import "."

Rectangle {
    id: button
    color: baseColor
    onEnabledChanged: checkColor()
    radius: Settings.buttonRadius

    property color baseColor: Settings.buttonColor
    property color pressedColor: Settings.buttonPressedColor
    property color disabledColor: Settings.disabledButtonColor

    signal clicked()

    function checkColor()
    {
        if (!button.enabled) {
            button.color = disabledColor
        } else {
            if (mouseArea.containsPress)
                button.color = pressedColor
            else
                button.color = baseColor
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onPressed: checkColor()
        onReleased: checkColor()
        onClicked: {
            checkColor()
            button.clicked()
        }
    }
}
