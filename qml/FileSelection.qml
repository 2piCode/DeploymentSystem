import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.0
import QtQuick.Window 2.12

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
        selectExisting: true
        selectMultiple: false
        onAccepted: {
            filepathField.text = utils.urlToLocalFile(fileDialog.fileUrl);
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
                radius: 4
                color: "white"
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
