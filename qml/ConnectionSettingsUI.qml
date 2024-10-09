import "../utils.js" as Utils
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQuick.Dialogs

ColumnLayout {
    id: station

    property bool isActive: false
    property int defaultSize: 50
    property int expandedSize: detailAreaLayout.implicitHeight
    property int expandedMarkerSize: 24
    readonly property int maxLabelWidth: Screen.width * 0.05
    readonly property int inputFieldWidth: Screen.width * 0.1
    property int fontSize: 12

    signal changedActivity(bool isActive)

    function changeActivity() {
        isActive = !isActive;
        changedActivity(isActive);
    }


    spacing: 0
    width: parent.width

    FileDialog {
        id: fileDialog
        title: "Select a Configuration File"
        nameFilters: ["Config files (*.pub)", "All files (*)"]

        onAccepted: {
            filepathField.text = fileDialog.currentFile
        }
        onRejected: {
            console.log("File selection was canceled")
        }
    }

    Item {
        id: header

        Layout.fillWidth: true
        Layout.preferredHeight: station.defaultSize

        Rectangle {
            id: background

            anchors.fill: parent
            color: "transparent"

            Text {
                id: settingsTitle

                text: "Настройки подключения"
                anchors.verticalCenter: parent.verticalCenter
                font.pointSize: fontSize + 2
            }

            Image {
                source: isActive ? "qrc:/images/images/minus.png" : "qrc:/images/images/plus.png"
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: settingsTitle.right
                anchors.leftMargin: 16
                width: expandedMarkerSize
                height: expandedMarkerSize
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    ipFieldEditor.visible = false;
                    ipField.visible = true;
                    changeActivity();
                }
            }
        }
    }

    Rectangle {
        id: detailArea
        opacity: 0

        Layout.fillWidth: true
        Layout.leftMargin: Screen.width * 0.025
        Layout.topMargin: 0
        

        color: "transparent"
        ColumnLayout {
            id: detailAreaLayout
            spacing: 5
            anchors.margins: 10
            width: station.width

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: loginField.height
                color: "transparent"
                border.color: "lightgray"
                border.width: 1

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10
                    Label {
                        Layout.preferredWidth: maxLabelWidth
                        font.pointSize: fontSize
                        text: "Логин:"
                    }
                    TextField {
                        id: loginField
                        Layout.preferredWidth: inputFieldWidth
                        font.pointSize: fontSize
                        placeholderText: "login"
                    }
                }
            }
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: passwordField.height
                color: "transparent"
                border.color: "lightgray"
                border.width: 1

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10

                    
                    Label {
                        Layout.preferredWidth: maxLabelWidth
                        font.pointSize: fontSize
                        text: "Пароль:"
                    }

                    TextField {
                        id: passwordField
                        Layout.preferredWidth: inputFieldWidth
                        font.pointSize: fontSize
                        placeholderText: "password"
                        echoMode: TextInput.Password
                    }
                }
            }
            ColumnLayout {
                    spacing: 15
                    Layout.fillWidth: true

                    CheckBox {
                        id: loadFromFile
                        font.pointSize: fontSize
                        text: "Конфигурация из файла"
                        checked: false
                        Layout.fillWidth: true
                        anchors.margins: 10
                    }
                    RowLayout {
                        visible: loadFromFile.checked
                        Layout.fillWidth: true
                        spacing: 10
                        TextField {
                            id: filepathField
                            Layout.preferredWidth: inputFieldWidth * 2
                            font.pointSize: fontSize
                            placeholderText: "path_to_your_config_file"
                        }
                        Button {
                            icon.source: "qrc:/images/images/openDir.png"
                            onClicked: {
                                fileDialog.open()
                            }
                        }
                    }
            }
        }

    }
    states: [
        State {
            name: "collapsed"
            when: !isActive
            PropertyChanges {
                target: detailArea
                Layout.preferredHeight: 0
            }
        },
        State {
            name: "expanded"
            when: isActive
            PropertyChanges {
                target: detailArea
                Layout.preferredHeight: station.expandedSize
            }
        }
    ]

    transitions: [
        Transition {
            from: "collapsed"
            to: "expanded"
            SequentialAnimation {
                NumberAnimation {
                    target: detailArea
                    property: "Layout.preferredHeight"
                    duration: 300
                    easing.type: Easing.InOutQuad
                }
                NumberAnimation {
                    target: detailArea
                    property: "opacity"
                    to: 1
                    duration: 0
                }
            }
        },
        Transition {
            from: "expanded"
            to: "collapsed"
            SequentialAnimation {
                NumberAnimation {
                    target: detailArea
                    property: "opacity"
                    to: 0
                    duration: 0
                }
                NumberAnimation {
                    target: detailArea
                    property: "Layout.preferredHeight"
                    to: 0
                    duration: 300
                    easing.type: Easing.InOutQuad
                }
            }
        }
    ]
}
