import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

ColumnLayout {
    id: station

    property bool isActive: false
    property int fontSize: Const.fontSize
    property int defaultSize: Const.stationItemHeigth
    property int expandedSize: detailAreaLayout.implicitHeight
    property int expandedMarkerSize: 20
    property int inputFieldsWidth: Screen.width * 0.1
    property string ip: "127.0.0.1"
    property string name: "Station Name"

    signal changedActivity(bool isActive)

    function changeActivity() {
        isActive = !isActive;
        changedActivity(isActive);
    }

    spacing: 0
    width: parent ? parent.width : 0

    Item {
        id: header

        Layout.fillWidth: true
        Layout.preferredHeight: station.defaultSize
        

        
        Rectangle {
            id: background
            anchors.fill: parent
            anchors{
                leftMargin: 10
                rightMargin: 20
            }
            color: "transparent"
            

            RowLayout {

                spacing: 10
                width: parent.width
                height: parent.height

                Text {
                    id: ipField
                    text: settingsDialog.switchState ? ip : name
                    font.pointSize: fontSize + 2

                    visible: true
                }


                Image {
                    source: isActive ? "qrc:/images/images/collapse.png" : "qrc:/images/images/expand.png"
                    // Layout.alignment: Qt.AlignVCenter
                    Layout.preferredWidth: expandedMarkerSize
                    Layout.preferredHeight: expandedMarkerSize
                    fillMode: Image.PreserveAspectFit
                }

                Item {
                    Layout.fillWidth: true
                }

                Image {
                    source: "qrc:/images/images/arm_engineer.png"
                    Layout.preferredHeight: defaultSize * 0.75
                    fillMode: Image.PreserveAspectFit

                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    changeActivity();
                }
            }
        }

    }

    Rectangle {
        id: detailArea
        opacity: 0

        Layout.fillWidth: true
        Layout.leftMargin: Screen.width * 0.025
        Layout.topMargin: 1

        color: "transparent"
        ColumnLayout {
            id: detailAreaLayout
            spacing: 15
            anchors.margins: 10
            width: station.width

            StationSettingsUI{
                inputFieldsWidth: station.inputFieldsWidth
                station: station
            }

            ConnectionSettingsUI {
                isActive: false
                inputFieldsWidth: station.inputFieldsWidth
            }

            InstallationSettingsUI{}
        }

    }
    states: [
        State {
            name: "collapsed"
            when: !isActive
            PropertyChanges {
                target: detailArea
                Layout.preferredHeight: 0
            }
        },
        State {
            name: "expanded"
            when: isActive
            PropertyChanges {
                target: detailArea
                Layout.preferredHeight: station.expandedSize
            }
        }
    ]

    transitions: [
        Transition {
            from: "collapsed"
            to: "expanded"
            SequentialAnimation {
                NumberAnimation {
                    target: detailArea
                    property: "Layout.preferredHeight"
                    duration: 300
                    easing.type: Easing.InOutQuad
                }
                NumberAnimation {
                    target: detailArea
                    property: "opacity"
                    to: 1
                    duration: 0
                }
            }
        },
        Transition {
            from: "expanded"
            to: "collapsed"
            SequentialAnimation {
                NumberAnimation {
                    target: detailArea
                    property: "opacity"
                    to: 0
                    duration: 0
                }
                NumberAnimation {
                    target: detailArea
                    property: "Layout.preferredHeight"
                    to: 0
                    duration: 300
                    easing.type: Easing.InOutQuad
                }
            }
        }
    ]
}
