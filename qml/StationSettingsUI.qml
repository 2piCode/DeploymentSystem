import "../utils.js" as Utils
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3


ColumnLayout {
    id: stationSettingsLayout
    property int fontSize: Const.fontSize
    property int inputFieldsWidth
    property var station

    Layout.preferredHeight: stationSettingsLayout.implicitHeight
    Layout.fillWidth: true
    spacing: 10

    Rectangle {
        Layout.fillWidth: true
        Layout.preferredHeight: stationNameLabel.height
        color: "transparent"

        RowLayout {
            Layout.fillWidth: true
            spacing: 10
            Label {
                id: stationNameLabel
                Layout.preferredWidth: Math.max(stationNameLabel.implicitWidth, ipLabel.implicitWidth)
                font.pointSize: fontSize
                text: qsTr("Имя станции")
            }
            TextField {
                id: stationNameField
                Layout.preferredWidth: inputFieldsWidth
                font.pointSize: fontSize
                background: Rectangle {
                    radius: 4
                    color: "white"
                }
                placeholderText: qsTr("имя станции")

                onEditingFinished: {
                    station.name = text;
                }
            }
        }
    }
    Rectangle {
        Layout.fillWidth: true
        Layout.preferredHeight: ipLabel.height
        color: "transparent"

        RowLayout {
            Layout.fillWidth: true
            spacing: 10


            Label {
                id: ipLabel
                Layout.preferredWidth: Math.max(stationNameLabel.implicitWidth, ipLabel.implicitWidth)
                font.pointSize: fontSize
                text: qsTr("IP адресс станции:")
            }

            TextField {
                id: passwordField
                Layout.preferredWidth: inputFieldsWidth
                font.pointSize: fontSize
                placeholderText: qsTr("ip")
                background: Rectangle {
                    border.color: "red"
                    border.width: 0
                    radius: 4
                    color: "white"
                }
                onTextChanged: {
                    background.border.width = Utils.isValidIP(text) ? 0 : 2  
                }
                onEditingFinished: {
                    if (Utils.isValidIP(text)){
                        station.ip = text;
                    }
                }

            }
        }
    }


    Label {
        id: descriptionLabel
        font.pointSize: fontSize
        text: qsTr("Описание:")
    }

    TextArea {
        id: descriptionField
        property int maxLines: 3
        Layout.preferredWidth: inputFieldsWidth * 3
        wrapMode: Text.Wrap        
        font.pointSize: fontSize
        placeholderText: qsTr("описание станции")

        onTextChanged: function() {
            if (lineCount > maxLines || text.length > 256) {
                text = text.substring(0,text.length - 1)
                cursorPosition = text.length
            }
        }
    }

}
