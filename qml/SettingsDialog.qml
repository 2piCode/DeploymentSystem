import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

import com.systems 1.0

Dialog {
    
    anchors.centerIn: parent
    title: qsTr("Настройки")
    property bool switchState: ipOverNameSwitch.checked
    font.pointSize: mainFontSize
    
    standardButtons: Dialog.Ok | Dialog.Cancel

    ColumnLayout {
        spacing: 15

        FileSelection{
            id: windowsInstaller
            fileSelectionTitle: qsTr("Путь до установщика Windows")
            additionalNameFilters: ".exe files (*.exe)"
        }
        FileSelection{
            id: astralinuxInstaller
            fileSelectionTitle: qsTr("Путь до установщика AstraLinux")
            additionalNameFilters: ".AppImage files (*.AppImage)"
        }
        FileSelection{
            id: redos7Installer
            fileSelectionTitle: qsTr("Путь до установщика RedOS 7")
            additionalNameFilters: ".AppImage files (*.AppImage)"

        }
        FileSelection{
            id: redos8Installer
            fileSelectionTitle: qsTr("Путь до установщика RedOS 8")
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
        const installers = [
            { system: Systems.System.Windows, path: windowsInstaller.filepath },
            { system: Systems.System.AstraLinux, path: astralinuxInstaller.filepath },
            { system: Systems.System.Redos7, path: redos7Installer.filepath },
            { system: Systems.System.Redos8, path: redos8Installer.filepath }
        ];

        installers.forEach(function(installer) {
            config.SetInstallerPath(installer.system, installer.path);
        });
            console.log("Settings accepted")
    }

    onRejected: {
        console.log("Settings canceled")
    }
}
