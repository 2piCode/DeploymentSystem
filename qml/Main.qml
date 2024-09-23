import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import "."

ApplicationWindow {
    visible: true
    title: qsTr("Конфигуратор станций")
    width: 800
    height: 800

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: console.log("Open action triggered");
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    Column {
        id: stations
        anchors.fill: parent
        spacing: 10

        ListView {
            id: listView
            width: parent.width
            height: parent.height

            property int selectedIndex: -1

            Behavior on height {
                NumberAnimation {
                    duration: 300
                    easing.type: Easing.InOutQuad
                }
            }

            model: ListModel {
                ListElement { ip: "127.0.0.1" }
                ListElement { ip: "127.0.0.1" }
                ListElement { ip: "127.0.0.1" }
                ListElement { ip: "127.0.0.1" }
            }

            delegate: StationItem {
                ip: model.ip
                onChangedActivity: function (isActive) {
                    if (isActive) {
                        listView.selectedIndex = index;
                        listView.currentIndex = index;
                    } else if (listView.selectedIndex === index) {
                        listView.selectedIndex = -1;
                    }
                }
            }

            Keys.onPressed: function(event) {
                if (event.key === Qt.Key_Return && event.modifiers === Qt.ControlModifier) {
                    ipDialog.open();
                }
            }
        }

        Button {
            // anchors.horizontalCenter: parent.horizontalCenter
            // anchors.top: stations.bottom
            onClicked: {
                ipDialog.open()
            }
        }
    }

    Dialog {
        id: ipDialog
        title: "Add New IP Address"
        anchors.centerIn: parent
        modal: true
        standardButtons: Dialog.Ok | Dialog.Cancel

        Column {
            spacing: 10
            padding: 10

            Text {
                text: "Enter new IP Address:"
            }

            TextField {
                id: newIpField
                placeholderText: "192.168.1.4"
                width: 200
            }
        }
        onAccepted: {
            var newIp = newIpField.text;
            if (isValidIP(newIp)) {
                listView.model.append({ ip: newIp });
            }
            newIpField.text = "";
        }
    }
}
