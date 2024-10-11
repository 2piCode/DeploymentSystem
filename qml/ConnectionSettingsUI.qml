import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

ColumnLayout {
    id: station

    property bool isActive: false
    property int defaultSize: 50
    property int expandedSize: detailAreaLayout.implicitHeight
    property int expandedMarkerSize: 16
    readonly property int maxLabelWidth: Screen.width * 0.05
    readonly property int loginWidth: Screen.width * 0.1
    property int fontSize: 12

    signal changedActivity(bool isActive)

    function changeActivity() {
        isActive = !isActive;
        changedActivity(isActive);
    }


    spacing: 0
    width: parent.width


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
                source: isActive ? "qrc:/images/images/collapse.png" : "qrc:/images/images/expand.png"
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: settingsTitle.right
                anchors.leftMargin: 12
                width: expandedMarkerSize
                height: expandedMarkerSize
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
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
                        Layout.preferredWidth: loginWidth
                        font.pointSize: fontSize
                        placeholderText: "login"
                    }
                }
            }
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: passwordField.height
                color: "transparent"

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
                        Layout.preferredWidth: loginWidth
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
                    FileSelection {
                        visible: loadFromFile.checked
                        inputFieldWidth: loginWidth * 2
                        additionalNameFilters: "файл конфигурации (*.pub)"
                        fileSelectionTitle: "Путь до файла конфигурации"
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
