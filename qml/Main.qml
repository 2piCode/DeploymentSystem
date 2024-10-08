import "."
import "../utils.js" as Utils
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

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
                onTriggered: console.log("Open action triggered")
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit()
            }
        }
    }

   header: ToolBar {
        id: mainToolBar
        RowLayout {
            Layout.alignment: Qt.AlignLeft
            spacing: 10

            CustomToolBarButton {
                id: saveBtn
                iconSource: "qrc:/images/images/save.png"
                onButtonClicked: {
                    console.log("Save button clicked");
                }
            }

            CustomToolBarButton {
                id: duplicateBtn
                iconSource: "qrc:/images/images/duplicate.png"
                onButtonClicked: {
                    console.log("Duplicate button clicked");
                }
            }

            CustomToolBarButton {
                id: prevBtn
                iconSource: "qrc:/images/images/back.png"
                onButtonClicked: {
                    console.log("Prev button clicked");
                }
            }

            CustomToolBarButton {
                id: nextBtn
                iconSource: "qrc:/images/images/next.png"
                onButtonClicked: {
                    console.log("Next button clicked");
                }
            }

            CustomToolBarButton {
                id: loadBtn
                iconSource: "qrc:/images/images/load.png"
                onButtonClicked: {
                    console.log("Load button clicked");
                }
            }

            CustomToolBarButton {
                id: uploadBtn
                iconSource: "qrc:/images/images/upload.png"
                onButtonClicked: {
                    console.log("Upload button clicked");
                }
            }

            Item {
                width: 30  // Создание пробела как в ТЗ
            }

            CustomToolBarButton {
                id: editBtn
                iconSource: "qrc:/images/images/edit.png"
                onButtonClicked: {
                    console.log("Edit button clicked");
                }
            }

            CustomToolBarButton {
                id: settingsBtn
                iconSource: "qrc:/images/images/settings.png"
                onButtonClicked: {
                    console.log("Settings button clicked");
                }
            }

            CustomToolBarButton {
                id: connectionBtn
                iconSource: "qrc:/images/images/connection.png"
                onButtonClicked: {
                    console.log("Connection button clicked");
                }
            }

            CustomToolBarButton {
                id: goBtn
                iconSource: "qrc:/images/images/go.png"
                onButtonClicked: {
                    console.log("Go button clicked");
                }
            }
        }
    }

    Column {
        id: stations

        spacing: 10

        anchors {
            top: header.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        ListView {
            id: listView

            property int selectedIndex: -1

            width: parent.width
            focus: true
            highlightMoveDuration: 100
            Keys.onPressed: function(event) {
                if (event.key === Qt.Key_Return && event.modifiers === Qt.ControlModifier)
                    ipDialog.open();
                else if (listView.currentIndex !== -1 && event.key === Qt.Key_Space){
                    currentItem.editIp();
                }
            }

            height: contentHeight

            highlight: Rectangle {
                color: "lightsteelblue"
                radius: 5
            }

            model: ListModel {
                id: listModel

                ListElement {ip: "127.0.0.1"}
                ListElement {ip: "127.0.0.1"}
                ListElement {ip: "127.0.0.1"}
            }

            delegate: StationItem {
                ip: model.ip
                onChangedActivity: function(isActive) {
                    if (listView.selectedIndex !== index) {
                        listView.selectedIndex = index;
                        listView.currentIndex = index;
                        currentItem.forceActiveFocus;
                    }
                }
            }

            Behavior on contentHeight {
                NumberAnimation {
                    duration: 300
                    easing.type: Easing.InOutQuad
                }

            }

        }

        Button {
            id: addIpBtn

            anchors.left: parent.left
            anchors.leftMargin: 3
            width: 60
            height: 40
            icon.source: "qrc:/images/images/plus.png"

            background: Rectangle {
                anchors.fill: parent
                color: "transparent"
                border.color: addIpBtn.pressed ? "gray" :
                     addIpBtn.hovered ? "darkgray" : "black"
                border.width: 2
                radius: 8

            }

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
        onAccepted: {
            var newIp = newIpField.text;
            if (Utils.isValidIP(newIp))
                listView.model.append({
                "ip": newIp
            });

            newIpField.text = "";
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
                
                Keys.onPressed: function(event) {
                    if (event.key === Qt.Key_Return)
                        ipDialog.accept();
                }
            }

        }
    }
}
