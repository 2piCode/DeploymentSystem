import QtQuick 2.12
import QtQuick.Controls.Basic 2.12
import QtQuick.Layouts 1.3

import com.stations 1.0

ColumnLayout {
    id: connectionSettingsLayout

    property Station station

    property bool isActive: false
    property int defaultSize: stationDefaultHeight * scalingFactor
    property int expandedSize: detailAreaLayout.implicitHeight
    property int expandedMarkerSize: 16
    readonly property int maxLabelWidth: Screen.width * 0.05
    property int inputFieldsWidth
    property int fontSize: mainFontSize

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
        Layout.preferredHeight: connectionSettingsLayout.defaultSize

        Rectangle {
            id: background

            anchors.fill: parent
            color: "transparent"

            Text {
                id: settingsTitle

                text: qsTr("Настройки подключения")
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
            width: connectionSettingsLayout.width

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: usernameField.height
                color: "transparent"

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10
                    Label {
                        id: loginLabel
                        Layout.preferredWidth: Math.max(loginLabel.implicitWidth, passwordLabel.implicitWidth)
                        font.pointSize: fontSize
                        text: qsTr("Имя пользователя:")
                    }
                    TextField {
                        id: usernameField
                        Layout.preferredWidth: inputFieldsWidth
                        font.pointSize: fontSize
                        placeholderText: qsTr("имя пользователя")
                        background: Rectangle {
                            color: "white"
                            radius: 5
                            border.width: 0 
                        }
                        text: station.username
                        onEditingFinished: {
                            station.SetUsername(text);
                        }
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
                        id: passwordLabel
                        Layout.preferredWidth: Math.max(loginLabel.implicitWidth, passwordLabel.implicitWidth)
                        font.pointSize: fontSize
                        text: qsTr("Пароль:")
                    }

                    TextField {
                        id: passwordField
                        Layout.preferredWidth: inputFieldsWidth
                        font.pointSize: fontSize
                        placeholderText: qsTr("пароль")
                        echoMode: TextInput.Password
                        background: Rectangle {
                            color: "white"
                            radius: 5
                            border.width: 0 
                        }
                        onEditingFinished: {    
                            station.SetPassword(text);
                        }

                    }
                }
            }
            ColumnLayout {
                spacing: 15
                Layout.fillWidth: true

                CheckBox {
                    id: loadFromFile
                    font.pointSize: fontSize
                    text: qsTr("Конфигурация из файла")
                    checked: false

                }
                FileSelection {
                    id: fileSelection
                    inputFieldWidth: inputFieldsWidth * 2
                    additionalNameFilters: "файл конфигурации (*.pub)"
                    fileSelectionTitle: qsTr("Путь до файла конфигурации")
                    
                    opacity: 0
                    Layout.preferredHeight: 0

                    states: [
                        State {
                            name: "visible"
                            when: loadFromFile.checked
                            PropertyChanges {
                                target: fileSelection
                                opacity: 1
                                Layout.preferredHeight: implicitHeight
                            }
                        },
                        State {
                            name: "hidden"
                            when: !loadFromFile.checked
                            PropertyChanges {
                                target: fileSelection
                                opacity: 0
                                Layout.preferredHeight: 0
                            }
                        }
                    ]

                    transitions: [
                        Transition {
                            from: "hidden"
                            to: "visible"
                            SequentialAnimation {
                                NumberAnimation {
                                    property: "Layout.preferredHeight"
                                    duration: 300
                                    easing.type: Easing.InOutQuad
                                }
                                NumberAnimation {
                                    property: "opacity"
                                    duration: 0
                                    easing.type: Easing.InOutQuad
                                }
                            }
                        },
                        Transition {
                            from: "visible"
                            to: "hidden"
                            SequentialAnimation {
                                NumberAnimation {
                                    property: "opacity"
                                    duration: 0
                                    easing.type: Easing.InOutQuad
                                }
                                NumberAnimation {
                                    property: "Layout.preferredHeight"
                                    duration: 300
                                    easing.type: Easing.InOutQuad
                                }
                            }
                        }
                    ]
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
                opacity: 0
            }
        },
        State {
            name: "expanded"
            when: isActive
            PropertyChanges {
                target: detailArea
                Layout.preferredHeight: connectionSettingsLayout.expandedSize
                opacity: 1
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
