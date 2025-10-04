import QtQuick 2.15
import QtQuick.Controls 2.15

Page {
    id: endPage
    anchors.fill: parent

    Rectangle {
        anchors.fill: parent
        color: "#f5f6fa"

        Column {
            anchors.centerIn: parent
            spacing: 40

            Text {
                text: "✅ Experiment Finished!"
                font.pixelSize: 36
                font.bold: true
                color: "#27ae60"
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                text: "Please shutdown the system."
                font.pixelSize: 24
                color: "#2f3640"
                horizontalAlignment: Text.AlignHCenter
            }

            Rectangle {
                width: 280
                height: 80
                radius: 20
                color: "#e84118"
                border.color: "#2f3640"
                border.width: 2

                Text {
                    anchors.centerIn: parent
                    text: "⏻ Shutdown System"
                    font.pixelSize: 26
                    font.bold: true
                    color: "white"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        Qt.quit()
                    }
                }
            }
        }
    }
}
