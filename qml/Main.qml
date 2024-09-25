import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import "."
import "../utils.js" as Utils

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
            focus: true

            property int selectedIndex: -1

            highlight: Rectangle {  color: "lightsteelblue"
                                    radius: 5
                                    height: StationItem.height

            }
            highlightMoveDuration: 100
            model: ListModel {
                id: listModel
                ListElement { ip: "127.0.0.1" }
                ListElement { ip: "127.0.0.1" }
                ListElement { ip: "127.0.0.1" }
                ListElement { ip: "127.0.0.1" }
            }

            delegate: StationItem {
                ip: model.ip
                onChangedActivity: function (isActive) {

                    if (listView.selectedIndex !== index) {
                            listView.selectedIndex = index;
                            listView.currentIndex = index;
                        }
                }
            }

            Keys.onPressed: function(event) {
                if (event.key === Qt.Key_Return && event.modifiers === Qt.ControlModifier) {
                    console.log("Try Focus changed");
                    ipDialog.open();
                    console.log("Focus changed");
                }
            }

            height: contentHeight
            Behavior on contentHeight {
                NumberAnimation {
                    duration: 300
                    easing.type: Easing.InOutQuad
                }
            }
        }


        Button {
            id: addIpBtn
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Add new IP address"
            onClicked: {
                    ipDialog.open();
            }
        }
    }

    Dialog {
        id: ipDialog
        title: "Add New IP Address"
        anchors.centerIn: parent
        modal: true
        standardButtons: Dialog.Ok | Dialog.Cancel

        onOpened: {
            newIpField.forceActiveFocus();
        }

        onClosed: {
            listView.forceActiveFocus();
        }

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
            if (Utils.isValidIP(newIp)) {
                listView.model.append({ ip: newIp });
            }
            newIpField.text = "";
        }
    }
}
