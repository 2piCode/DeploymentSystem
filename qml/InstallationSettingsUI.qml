import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

ColumnLayout{
    property int fontSize: Const.fontSize

    Rectangle {
        
        Layout.fillWidth: true
        Layout.preferredHeight: roleSelectionLayout.implicitHeight
        color: "transparent"

        ColumnLayout {
            id: roleSelectionLayout
            Layout.fillWidth: true
            spacing: 10

            Label {
                text: qsTr("Роль:")
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
                text: qsTr("Дополнительные опции")
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
