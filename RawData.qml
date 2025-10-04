import QtQuick 2.15
import QtQuick.Controls 2.15
import "DataManager.js" as DataManager

Page {
    id: rawDataPage
    anchors.fill: parent

    Rectangle {
        anchors.fill: parent
        color: "#f5f6fa"

        Column {
            anchors.centerIn: parent
            spacing: 20

            Text {
                text: "📊 Raw Data (Dummy PCR)"
                font.pixelSize: 42
                font.bold: true
                color: "#2f3640"
                horizontalAlignment: Text.AlignHCenter
            }

            Rectangle {
                width: 700
                height: 400
                color: "white"
                border.color: "#dcdde1"
                radius: 10
                clip: true

                Column {
                    anchors.fill: parent

                    Row {
                        spacing: 40
                        padding: 10
                        Rectangle {
                            width: 80; height: 40
                            color: "#dcdde1"
                            Text {
                                anchors.centerIn: parent
                                text: "Cycle"
                                font.bold: true
                                font.pixelSize: 25
                            }
                        }
                        Rectangle {
                            width: 220; height: 40
                            color: "#dcdde1"
                            Text {
                                anchors.centerIn: parent
                                text: "Fluorescence (RFU)"
                                font.bold: true
                                font.pixelSize: 25
                            }
                        }
                        Rectangle {
                            width: 220; height: 40
                            color: "#dcdde1"
                            Text {
                                anchors.centerIn: parent
                                text: "Temperature (°C)"
                                font.bold: true
                                font.pixelSize: 25
                            }
                        }
                    }

                    ListView {
                        id: rawDataList
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        anchors.topMargin: 50
                        height: parent.height - 50
                        clip: true

                        ScrollBar.vertical: ScrollBar {
                            policy: ScrollBar.AlwaysOn
                        }

                        model: ListModel {
                            Component.onCompleted: {
                                var data = DataManager.getRawData();
                                for (var i = 0; i < data.length; i++) {
                                    append({
                                        "cycle": data[i].cycle,
                                        "fluorescence": data[i].fluorescence,
                                        "temp": data[i].temperature
                                    })
                                }
                            }
                        }

                        delegate: Row {
                            spacing: 40
                            padding: 10
                            Rectangle {
                                width: 80; height: 35
                                color: "transparent"
                                Text { anchors.centerIn: parent; text: cycle; font.pixelSize: 25 }
                            }
                            Rectangle {
                                width: 220; height: 35
                                color: "transparent"
                                Text { anchors.centerIn: parent; text: fluorescence; font.pixelSize: 25 }
                            }
                            Rectangle {
                                width: 220; height: 35
                                color: "transparent"
                                Text { anchors.centerIn: parent; text: temp + " °C"; font.pixelSize: 25 }
                            }
                        }
                    }
                }
            }
        }
    }
}

