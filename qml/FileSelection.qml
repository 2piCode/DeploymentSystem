import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQuick.Dialogs

ColumnLayout{
    property string fileSelectionTitle: "Путь до файла"
    property string additionalNameFilters;
    property int inputFieldWidth: Screen.width * 0.15
    property int fontSize: 12


    FileDialog {
        id: fileDialog
        title: "Select a Configuration File"
        nameFilters: [additionalNameFilters, "All files (*)"]

        onAccepted: {
            filepathField.text = fileDialog.currentFile
        }
    }

    Text{
        Layout.preferredWidth: inputFieldWidth
        font.pointSize: fontSize 
        text: fileSelectionTitle
    }

    RowLayout {
        
        Layout.fillWidth: true
        spacing: 2
        TextField {
            id: filepathField
            Layout.preferredWidth: inputFieldWidth
            font.pointSize: fontSize
            placeholderText: qsTr("Path to file")
                background: Rectangle {
                color: "white"
                radius: 5
                border.width: 0 
            }
        }
        Button {
            icon.source: "qrc:/images/images/openDir.png"
            flat: true
            Layout.preferredHeight: filepathField.height
            onClicked: {
                fileDialog.open()
            }
        }
    }


}
