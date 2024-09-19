import QtQuick 2.12
import QtQuick.Controls 2.12

ApplicationWindow {
    visible: true
    title: qsTr("Конфигуратор станций")
    width: 427
    height: 480

    function isValidIP(ip) {
        var ipPattern = /^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/;
        return ipPattern.test(ip);
    }

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
        anchors.fill: parent
        spacing: 10

        ListView {
            id: listView
            width: parent.width
            height: listView.selectedIndex === -1 ? parent.height - 100 : parent.height - 150
            Behavior on height {
                NumberAnimation {
                    duration: 300
                    easing.type: Easing.InOutQuad
                }
            }

            model: ListModel {
                ListElement { ip: "127.0.0.1" }
                ListElement { ip: "192.168.1.1" }
                ListElement { ip: "10.0.0.1" }
            }
            property int selectedIndex: -1
            delegate: Column {
                width: listView.width
                Item {
                    width: parent.width
                    height: 40
                    focus: true

                    Rectangle {
                        id: background
                        anchors.fill: parent
                        color: (listView.currentIndex === index || listView.selectedIndex === index) ? "lightblue" : "transparent"

                        TextField {
                            id: ipFieldEditor
                            text: ip
                            anchors.verticalCenter: parent.verticalCenter
                            width: parent.width
                            visible: false

                            onEditingFinished: {
                                if (isValidIP(ipFieldEditor.text)) {
                                    model.ip = ipFieldEditor.text;
                                }
                                ipFieldEditor.visible = false;
                                ipField.visible = true;
                            }
                        }

                        Text {
                            id: ipField
                            text: ip
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    if (listView.selectedIndex === index) {
                                        // If the same item is clicked, hide the detail area
                                        listView.selectedIndex = -1;
                                    } else {
                                        // Otherwise, show the new detail area
                                        listView.selectedIndex = index;
                                    }
                                    listView.currentIndex = index;
                                }
                                onDoubleClicked: {
                                    ipField.visible = false;
                                    ipFieldEditor.visible = true;
                                    ipFieldEditor.forceActiveFocus();
                                }
                            }
                        }
                    }
                }

                Rectangle {
                    id: detailArea
                    width: parent.width
                    height: listView.selectedIndex === index ? 50 : 0
                    Behavior on height {
                        NumberAnimation {
                            duration: 300
                            easing.type: Easing.InOutQuad
                        }
                    }
                    color: "lightgray"
                    visible: listView.selectedIndex === index
                    Text {
                        anchors.centerIn: parent
                        text: "Selected IP: " + ip
                    }
                }

            }
        }

        // Key event handler for opening the dialog
        Keys.onPressed: function(event) {
            if (event.key === Qt.Key_Return && event.modifiers === Qt.ControlModifier) {
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
            newIpField.text = ""; // Clear the input field
        }
    }
}
