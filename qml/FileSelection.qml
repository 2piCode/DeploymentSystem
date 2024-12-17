import QtQuick 2.12
import QtQuick.Controls.Basic 2.12
import QtQuick.Layouts 1.3
import QtQuick.Dialogs

ColumnLayout{
    property string fileSelectionTitle: "Путь до файла"
    property string additionalNameFilters;
    property int inputFieldWidth: Screen.width * 0.15
    property int fontSize: mainFontSize;
    property string filepath;


    FileDialog {
        id: fileDialog
        title: "Select a Configuration File"
        nameFilters: [additionalNameFilters, "All files (*)"]

        onAccepted: {
            filepathField.text = utils.urlToLocalFile(fileDialog.currentFile);
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
            text: filepath
            onTextChanged: filepath = text;
        }
        Button {
            icon.source: "qrc:/images/images/openDir.png"
            padding: 0
            flat: true
            Layout.preferredHeight: filepathField.height
            Layout.preferredWidth: 50 * scalingFactor
            onClicked: {
                fileDialog.open()
            }
        }
    }


}
