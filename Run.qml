import QtQuick 2.15
import QtQuick.Controls 2.15

Page {
    id: experimentPage
    anchors.fill: parent

    property int cycle: 0
    property int maxCycle: 40
    property int elapsed: 0

    Timer {
        id: cycleTimer
        interval: 1000
        repeat: true
        running: false
        onTriggered: {
            elapsed += 1
            if (cycle < maxCycle) {
                cycle += 1
            } else {
                cycleTimer.stop()
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "#f5f6fa"

        Column {
            anchors.centerIn: parent
            spacing: 40

            Text {
                id: statusText
                text: cycleTimer.running
                      ? "🧪 Experiment is running..."
                      : (cycle >= maxCycle ? "✅ Experiment finished!" : "Press Start to run")
                font.pixelSize: 32
                font.bold: true
                color: "#2f3640"
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                text: "⏳ Elapsed Time: " + elapsed + " s"
                font.pixelSize: 28
                color: "#192a56"
            }

            Rectangle {
                id: startButton
                width: 300
                height: 80
                radius: 20
                color: cycleTimer.running ? "#718093" : "#44bd32"
                border.color: "#2f3640"
                border.width: 2

                Text {
                    anchors.centerIn: parent
                    text: cycleTimer.running ? "Running..." : "▶ Start Experiment"
                    font.pixelSize: 26
                    font.bold: true
                    color: "white"
                }

                MouseArea {
                    anchors.fill: parent
                    enabled: !cycleTimer.running
                    onClicked: {
                        cycle = 0
                        elapsed = 0
                        cycleTimer.start()
                    }
                }
            }
        }
    }
}
