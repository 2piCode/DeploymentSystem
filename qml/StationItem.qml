import "../utils.js" as Utils
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

Column {
    id: station

    property bool isActive: false
    property int defaultSize: 80
    property int expandedSize: 80
    property int expandedMarkerSize: 24
    property string ip: "127.0.0.1"

    signal changedActivity(bool isActive)

    function changeActivity() {
        console.log(isActive);
        console.log(detailArea.height);
        isActive = !isActive;
        console.log(isActive);
        console.log(detailArea.height);
        changedActivity(isActive);
    }

    width: parent.width

    Item {
        id: header

        width: parent.width
        height: station.defaultSize

        Rectangle {
            id: background

            anchors.fill: parent
            color: "transparent"
            height: parent.height

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
                id: expandedMarker

                source: isActive ? "../images/minus.png" : "../images/plus.png"
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
                    ipField.visible = false;
                    ipFieldEditor.visible = true;
                    ipFieldEditor.forceActiveFocus();
                }
            }

        }

    }

    Rectangle {
        id: detailArea

        width: parent.width
        height: isActive ? station.expandedSize : 0
        color: "lightgray"

        Text {
            visible: isActive
            anchors.centerIn: parent
            text: "Selected IP: " + ip
        }

        Behavior on height {
            NumberAnimation {
                duration: 300
                easing.type: Easing.InOutQuad
            }

        }

    }

}
