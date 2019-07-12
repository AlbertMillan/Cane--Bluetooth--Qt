import QtQuick 2.12
import QtQuick.Window 2.12
import Settings 1.0
import "."

// Bind bluetooth with button


Window {
    visible: true
    width: 720 * .7
    height: 1240 * .7
    title: qsTr("Hello World")
    color: Settings.backgroundColor

    Component.onCompleted: {
//        Settings.wWidth = Qt.binding(function() {return width})
//        Settings.wHeight = Qt.binding(function() {return height})
    }

    Loader {
        id: appLoader
        anchors.fill: parent
        asynchronous: false
        visible: true
        source: "App.qml"
        onStatusChanged: {
            if( status == Loader.Ready ){
                appLoader.item.init()
//                appLoader.item.forceActiveFocus();
            }
        }
    }
}
