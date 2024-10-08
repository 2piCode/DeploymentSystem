import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

RowLayout  {
    id: connectionSettings

    Layout.fillWidth: true
    property int expandedMarkerSize: 18
    spacing: 10

    property bool isActive: false

    Text {
        text: "Настройки подключения"
        Layout.alignment: Qt.AlignVCenter
        Layout.fillWidth: true
    }
    Image {
        id: tmpImgId
        source: isActive ? "qrc:/images/images/minus.png" : "qrc:/images/images/plus.png"
        Layout.preferredWidth: expandedMarkerSize
        Layout.preferredHeight: expandedMarkerSize
        Layout.alignment: Qt.AlignVCenter
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            console.log(tmpImgId.implicitHeight, tmpImgId.implicitHeight);
        }
        onDoubleClicked: {
            console.log("Double clicked");
        }
    }

}
