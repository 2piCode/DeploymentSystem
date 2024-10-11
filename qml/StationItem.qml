import "../utils.js" as Utils
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

ColumnLayout {
    id: station

    property bool isActive: false
    property int defaultSize: 50
    property int expandedSize: detailAreaLayout.implicitHeight
    property int expandedMarkerSize: 16
    property int fontSize: 12
    property string ip: "127.0.0.1"

    signal changedActivity(bool isActive)

    function changeActivity() {
        isActive = !isActive;
        changedActivity(isActive);
    }

    function editIp() {
        ipField.visible = false;
        ipFieldEditor.visible = true;
        ipFieldEditor.forceActiveFocus();
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

            TextField {
                id: ipFieldEditor

                text: ip
                anchors.verticalCenter: parent.verticalCenter
                visible: false
                font.pointSize: fontSize + 2
                onEditingFinished: {
                    if (Utils.isValidIP(ipFieldEditor.text))
                        ip = ipFieldEditor.text;

                    ipFieldEditor.visible = false;
                    ipField.visible = true;
                    listView.currentItem.forceActiveFocus()
                }
            }

            Text {
                id: ipField

                text: ip
                anchors.verticalCenter: parent.verticalCenter
                font.pointSize: fontSize + 2
            }

            Image {
                source: isActive ? "qrc:/images/images/collapse.png" : "qrc:/images/images/expand.png"
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: ipFieldEditor.visible ? ipFieldEditor.right : ipField.right
                anchors.leftMargin: 10
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
                onDoubleClicked: {
                    editIp();
                }
            }
        }
    }

    Rectangle {
        id: detailArea
        opacity: 0

        Layout.fillWidth: true
        Layout.leftMargin: Screen.width * 0.025
        Layout.topMargin: 1

        color: "transparent"
        ColumnLayout {
            id: detailAreaLayout
            spacing: 15
            anchors.margins: 10
            width: station.width


            ConnectionSettingsUI {
                isActive: false
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: roleSelectionLayout.implicitHeight
                color: "transparent"

                ColumnLayout {
                    id: roleSelectionLayout
                    Layout.fillWidth: true
                    spacing: 10

                    Label {
                        text: "Роль:"
                        font.pointSize: fontSize + 2
                    }
                    ComboBox {
                        Layout.minimumWidth: Screen.width * 0.2
                        font.pointSize: fontSize
                        model: ["АРМ инженера", "АРМ оператора", "Сервер", "Экспертный режим"]
                        delegate: ItemDelegate {
                            text: modelData
                            font.pointSize: fontSize
                            Layout.fillWidth: true

                            padding: 5
                        }
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: additionalOptionsLayout.implicitHeight
                color: "transparent"

                ColumnLayout {
                    id: additionalOptionsLayout
                    spacing: fontSize
                    Layout.fillWidth: true

                    Label {
                        text: "Дополнительные опции"
                        font.pointSize: fontSize + 2
                    }

                    CheckBox {
                        text: "Первая опция"
                        font.pointSize: fontSize
                        checked: true
                        Layout.fillWidth: true
                    }
                    CheckBox {
                        text: "Вторая опция"
                        font.pointSize: fontSize
                        checked: true
                        Layout.fillWidth: true
                    }
                    CheckBox {
                        text: "Третья опция"
                        font.pointSize: fontSize
                        checked: true
                        Layout.fillWidth: true
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
