import "../utils.js" as Utils
import QtQuick 2.12
import QtQuick.Controls.Basic 2.12
import QtQuick.Layouts 1.3


ColumnLayout {
    id: stationSettingsLayout
    property int fontSize: mainFontSize
    property int inputFieldsWidth
    property var station

    Layout.preferredHeight: stationSettingsLayout.implicitHeight
    Layout.fillWidth: true
    spacing: 20

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
                text: station.name

                onEditingFinished: {
                    station.SetName(text)
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
                id: hostNameField
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
                text: station.hostName
                onEditingFinished: {
                    if (Utils.isValidIP(text)){
                        station.SetHostName(text)
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
        property int maxLines: 4
        Layout.preferredWidth: inputFieldsWidth * 3
        wrapMode: Text.Wrap        
        font.pointSize: fontSize
        text: station.description
        placeholderText: qsTr("описание станции")

        background: Rectangle {
            color: "white"
            radius: 5
            border.width: 0 
        }

        onTextChanged: function() {
            if (lineCount > maxLines || text.length > 256) {
                text = text.substring(0,text.length - 1)
                cursorPosition = text.length
            }
        }
    }

}
