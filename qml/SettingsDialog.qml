import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

Dialog {
    
    anchors.centerIn: parent
    title: "Settings"
    font.pointSize: 12
    modal: true
    width: parent.width * 0.5
    
    standardButtons: Dialog.Ok | Dialog.Cancel

    ColumnLayout {
        spacing: 15

        FileSelection{
            fileSelectionTitle: "Путь до устанощика Windows"
            additionalNameFilters: ".exe files (*.exe)"
        }
        FileSelection{
            fileSelectionTitle: "Путь до устанощика AstraLinux"
            additionalNameFilters: ".AppImage files (*.AppImage)"
        }
        FileSelection{
            fileSelectionTitle: "Путь до устанощика RedOS 7"
            additionalNameFilters: ".AppImage files (*.AppImage)"

        }
        FileSelection{
            fileSelectionTitle: "Путь до устанощика RedOS 8"
            additionalNameFilters: ".AppImage files (*.AppImage)"
        }

        Text {
            text: "Язык"
            font.pointSize: 12
            font.bold: true
        }

        ComboBox {
            id: languageComboBox
            model: ["Русский", "English"]
            Layout.preferredWidth: parent.width * 0.5
            currentIndex: 0
        }
    }


    onAccepted: {
        console.log("Settings accepted")
    }

    onRejected: {
        console.log("Settings canceled")
    }
}
