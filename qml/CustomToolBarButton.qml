// CustomToolButton.qml
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

ToolButton {
    id: customToolButton

    property int buttonSize: parent.buttonSize ? parent.buttonSize : 48
    property string iconSource: ""
    signal buttonClicked()

    icon{
        source: iconSource
        width: customToolButton.buttonSize
        height: customToolButton.buttonSize
        }

    Layout.preferredWidth: buttonSize
    Layout.preferredHeight: buttonSize

    onClicked: {
        buttonClicked()
    }
}
