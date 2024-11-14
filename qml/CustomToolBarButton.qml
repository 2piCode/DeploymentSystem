import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

ToolButton {
    id: customToolButton

    property int baseButtonSize: 50

    property int buttonSize: baseButtonSize * scalingFactor
    
    property string iconSource: ""
    signal buttonClicked()

    icon.source: iconSource
    icon.width: buttonSize
    icon.height: buttonSize

    Layout.preferredWidth: buttonSize
    Layout.preferredHeight: buttonSize

    onClicked: {
        buttonClicked()
    }
}
