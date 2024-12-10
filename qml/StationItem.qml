import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

import com.stations 1.0
import com.roles 1.0

ColumnLayout {
    id: stationItem 

    property Station station
    property bool isActive: false
    property int fontSize: mainFontSize
    property int defaultSize: stationDefaultHeight * scalingFactor
    property int expandedSize: detailAreaLayout.implicitHeight
    property int expandedMarkerSize: (stationDefaultHeight / 2) * scalingFactor
    property int inputFieldsWidth: 200 * scalingFactor

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
        Layout.preferredHeight: stationItem.defaultSize
        

        
        Rectangle {
            id: background
            anchors.fill: parent
            anchors{
                leftMargin: 10
                rightMargin: 27
            }
            color: "transparent"
            

            RowLayout {

                spacing: 10
                width: parent.width
                height: parent.height

                Text {
                    id: ipField
                    text: settingsDialog.switchState ? station.hostName : station.name
                    font.pointSize: fontSize + 2

                    visible: true
                }


                Image {
                    source: isActive ? "qrc:/images/images/collapse.png" : "qrc:/images/images/expand.png"
                    Layout.preferredWidth: expandedMarkerSize
                    Layout.preferredHeight: expandedMarkerSize
                    fillMode: Image.PreserveAspectFit
                }

                Item {
                    Layout.fillWidth: true
                }

                Image {
                    source: {
                        switch (station.role){
                            case Roles.Role.arm_engineer:
                                return "qrc:/images/images/arm_engineer.png";
                            case Roles.Role.arm_operator:
                                return "qrc:/images/images/arm_operator.png";
                            case Roles.Role.arm_kip:
                                return "qrc:/images/images/arm_kip.png";
                            case Roles.Role.input_output_server:
                                return "qrc:/images/images/io_server.png";
                            case Roles.Role.history_server:
                                return "qrc:/images/images/history_server.png";
                            case Roles.Role.integration_server:
                                return "qrc:/images/images/integration_server.png";
                        }
                    }

                    Layout.preferredHeight: stationDefaultHeight * scalingFactor
                    fillMode: Image.PreserveAspectFit

                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.log( station.hostName)
                    console.log( station.name)
                    console.log( station.role)
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
            width: stationItem.width

            StationSettingsUI{
                inputFieldsWidth: stationItem.inputFieldsWidth
                station: stationItem.station
            }

            ConnectionSettingsUI {
                station: stationItem.station
                isActive: false
                inputFieldsWidth: stationItem.inputFieldsWidth
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
                Layout.preferredHeight: stationItem.expandedSize
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
