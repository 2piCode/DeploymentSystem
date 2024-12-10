import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import com.roles 1.0

ColumnLayout{
    property int fontSize: mainFontSize

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
                id: roleSelection
                Layout.minimumWidth: Screen.width * 0.2
                font.pointSize: fontSize
                model: Roles.getAllRoles()
                textRole: "name"
                delegate: ItemDelegate {
                    text: modelData.name
                    font.pointSize: fontSize
                    Layout.fillWidth: true

                    padding: 5
                }
                
                onCurrentIndexChanged: {
                    station.SetRole(currentIndex)

                }

                Connections {
                    target: languageController
                    function onLanguageChanged() {
                        var currentValue = roleSelection.currentIndex
                        roleSelection.model = Roles.getAllRoles()
                        roleSelection.currentIndex = currentValue
                    }
                }
                onModelChanged: {
                    roleSelection.currentIndex = station.role

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
