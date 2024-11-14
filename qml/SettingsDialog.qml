import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

Dialog {
    
    anchors.centerIn: parent
    title: qsTr("Настройки")
    property bool switchState: ipOverNameSwitch.checked
    font.pointSize: mainFontSize
    
    standardButtons: Dialog.Ok | Dialog.Cancel

    ColumnLayout {
        spacing: 15

        FileSelection{
            fileSelectionTitle: qsTr("Путь до устанощика Windows")
            additionalNameFilters: ".exe files (*.exe)"
        }
        FileSelection{
            fileSelectionTitle: qsTr("Путь до устанощика AstraLinux")
            additionalNameFilters: ".AppImage files (*.AppImage)"
        }
        FileSelection{
            fileSelectionTitle: qsTr("Путь до устанощика RedOS 7")
            additionalNameFilters: ".AppImage files (*.AppImage)"

        }
        FileSelection{
            fileSelectionTitle: qsTr("Путь до устанощика RedOS 8")
            additionalNameFilters: ".AppImage files (*.AppImage)"
        }


        Switch{
            id: ipOverNameSwitch
            checked: true
            text: qsTr("Перечисление станции по IP адресу") 
        }

        Text {
            text: qsTr("Язык")
            font.pointSize: mainFontSize
            font.bold: true
        }

        ComboBox {
            id: languageComboBox
            model: ["Русский", "English"]
            currentIndex: 0
            onCurrentIndexChanged: {
                languageController.switchLanguage();
            }
        }
    }


    onAccepted: {
        console.log("Settings accepted")
    }

    onRejected: {
        console.log("Settings canceled")
    }
}
