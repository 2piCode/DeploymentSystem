import "../utils.js" as Utils
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

ColumnLayout {
    id: station

    property bool isActive: false
    property int defaultSize: 50
    property int expandedSize: detailAreaLayout.implicitHeight
    property int expandedMarkerSize: 24
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
                font.pointSize: 16
                onEditingFinished: {
                    if (Utils.isValidIP(ipFieldEditor.text))
                        ip = ipFieldEditor.text;

                    ipFieldEditor.visible = false;
                    ipField.visible = true;
                }
            }

            Text {
                id: ipField

                text: ip
                anchors.verticalCenter: parent.verticalCenter
                font.pointSize: 16
            }

            Image {
                source: isActive ? "qrc:/images/images/minus.png" : "qrc:/images/images/plus.png"
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: ipFieldEditor.visible ? ipFieldEditor.right : ipField.right
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
            spacing: 5
            anchors.margins: 10
            width: station.width


            ConnectionSettingsUI {
                isActive: false
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 45
                color: "transparent"
                border.color: "lightgray"
                border.width: 1

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 10

                    Label {
                        text: "Роль:"
                        Layout.alignment: Qt.AlignVCenter
                    }
                    ComboBox {
                        Layout.minimumWidth: 200
                        model: ["АРМ инженера", "АРМ оператора", "Сервер", "Экспертный режим"]
                        anchors.margins: 10
                    }
                }
            }

            GroupBox {
                title: "Дополнительные задачи"
                Layout.fillWidth: true
                padding: 0

                ColumnLayout {
                    spacing: 15
                    Layout.fillWidth: true

                    CheckBox {
                        text: "Первая опция"
                        checked: true
                        Layout.fillWidth: true
                        anchors.margins: 10
                    }
                    CheckBox {
                        text: "Вторая опция"
                        checked: true
                        Layout.fillWidth: true
                        anchors.margins: 10
                    }
                    CheckBox {
                        text: "Третья опция"
                        checked: true
                        Layout.fillWidth: true
                        anchors.margins: 10
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
