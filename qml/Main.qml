import "."
import "../utils.js" as Utils
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3


ApplicationWindow {
    id: mainWindow
    visible: true
    title: qsTr("Конфигуратор станций")
    width: 800
    height: 800

    function deleteStation() {

        listModel.remove(listView.currentIndex);
        listView.selectedIndex = -1;
        listView.currentIndex = -1;

    }

    SettingsDialog {
        id: settingsDialog
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("Файл")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: console.log("Open action triggered")
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit()
            }
        }
        Menu {
            title: qsTr("Проект")
            MenuItem {
                text: qsTr("Импорт")
                onTriggered: console.log("Open action triggered")
            }
            MenuItem {
                text: qsTr("Экспорт")
                onTriggered: Qt.quit()
            }
            MenuItem {
                text: qsTr("Настройки")
                onTriggered: settingsDialog.open();
            }
        }
    }

    header: ToolBar {
        id: mainToolBar
        RowLayout {
            spacing: 10
            width: parent.width

            CustomToolBarButton {
                id: saveBtn
                iconSource: "qrc:/images/images/save.png"
                onButtonClicked: {
                    console.log("Save button clicked");
                    console.log(mainWindow.width, mainWindow.height);
                    console.log(Screen.width, Screen.height);
                }
            }

            CustomToolBarButton {
                id: duplicateBtn
                iconSource: "qrc:/images/images/duplicate.png"
                onButtonClicked: {
                    console.log("Duplicate button clicked");
                    console.log(Constants.fontSize);
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

            Item {
                Layout.preferredWidth: Screen.width * 0.02
            }

            CustomToolBarButton {
                id: editBtn
                iconSource: "qrc:/images/images/edit.png"
                onButtonClicked: {
                    console.log("Edit button clicked");
                    languageController.switchLanguage()
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
                    console.log(Const.stationItemHeigth);
                }
            }
            
            Item {
                Layout.fillWidth: true
            }

            CustomToolBarButton {
                id: addBtn
                iconSource: "qrc:/images/images/plus.png"
                onButtonClicked: {
                    ipDialog.open();
                }
            }

            CustomToolBarButton {
                id: deleteBtn
                iconSource: "qrc:/images/images/delete.png"
                onButtonClicked: {
                    deleteStation();    
                }
            }
        }
    }

    ColumnLayout {
        id: stations
        spacing: 10
        width: parent.width
        height: parent.height
        Layout.fillWidth: true
        Layout.fillHeight: true

        ListView {
            id: listView
            property int selectedIndex: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            focus: true
            clip: true
            highlightFollowsCurrentItem: true
            highlightResizeVelocity: Infinity
            highlightMoveDuration: 300

            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AsNeeded
            }

            Keys.onPressed: function(event) {
                if (event.key === Qt.Key_Return && event.modifiers === Qt.ControlModifier)
                    ipDialog.open();
                else if (listView.currentIndex !== -1 && event.key === Qt.Key_Space){
                    currentItem.editIp();
                }
                else if (listView.currentIndex !== -1 && event.key === Qt.Key_Delete){
                    deleteStation();
                }
            }

            highlight: Item {
                Rectangle {
                    color: "lightsteelblue"
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: Const.stationItemHeigth
                }
            }
                

            model: ListModel {
                id: listModel
                ListElement { ip: "127.0.0.1" }
                ListElement { ip: "127.0.0.2" }
                ListElement { ip: "127.0.0.3" }
            }

            delegate: StationItem {
                ip: model.ip
                Layout.fillWidth: true

                onChangedActivity: function(isActive) {
                    if (listView.selectedIndex !== index) {
                        listView.selectedIndex = index;
                        listView.currentIndex = index;
                        listView.currentItem.forceActiveFocus();
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
    }

    Dialog {
        id: ipDialog

        title: qsTr("Add New IP Address")
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
            if (Utils.isValidIP(newIp)) {
                listView.model.append({
                "ip": newIp
                });
            }

            newIpField.text = "";
        }
        Column {
            spacing: 10
            padding: 10

            Text {
                text: qsTr("Enter new IP Address:")
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
