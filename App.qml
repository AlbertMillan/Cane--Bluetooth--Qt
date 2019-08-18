import QtQuick 2.12
import Settings 1.0
import "."

Item {
    id: app
    anchors.fill: parent
    opacity: 0.0

    Behavior on opacity { NumberAnimation { duration: 500 } }

    property var lastPages: []
    property int __currentIndex: 0

    function init()
    {
        opacity = 1.0
//        showPage("PageConnect.qml")
        showPage("PageRegister.qml")
    }

    function prevPage()
    {
        lastPages.pop()
        pageLoader.setSource(lastPages[lastPages.length-1])
        __currentIndex = lastPages.length-1;
    }

    function showPage(name)
    {
        lastPages.push(name)
        pageLoader.setSource(name)
        __currentIndex = lastPages.length-1;
    }

    Loader {
        id: pageLoader
        anchors.fill: parent
        source: ""
        onStatusChanged: {
            if (status === Loader.Ready)
            {
                pageLoader.item.init();
                pageLoader.item.forceActiveFocus();
            }
        }
    }

    Keys.onReleased: {
        switch (event.key) {
        case Qt.Key_Escape:
        case Qt.Key_Back: {
            if (__currentIndex > 0) {
                pageLoader.item.close()
                event.accepted = true
            } else {
                Qt.quit()
            }
            break;
        }
        default: break;
        }
    }

//    BluetoothAlarmDialog {
//        id: btAlarmDialog
//        anchors.fill: parent
//        visible: !connectionHandler.alive
//    }

}
