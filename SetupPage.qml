import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: setupPage
    anchors.fill: parent
    color: "#fdfdfd"
    radius: 20

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 30

        Text {
            text: "⚙ Setup Parameters"
            font.family: "Segoe UI"
            font.pixelSize: 36
            font.bold: true
            color: "#2c3e50"
            horizontalAlignment: Text.AlignHCenter
            Layout.alignment: Qt.AlignHCenter
        }

        ColumnLayout {
            spacing: 20
            Label {
                text: "Intensity:"
                font.family: "Segoe UI"
                font.pixelSize: 36
                color: "#34495e"
            }
            Slider {
                id: intensitySlider
                from: 0
                to: 100
                value: 50
                stepSize: 5

                Layout.fillWidth: true
                Layout.preferredWidth: 500
                Layout.minimumWidth: 400
            }

            Text {
                text: "Light Intensity Level: " + intensitySlider.value + "%"
                font.family: "Segoe UI"
                font.pixelSize: 36
                color: "#7f8c8d"
            }
        }

        Button {
            text: "💾 Apply Settings"
            font.family: "Segoe UI"
            font.pixelSize: 32
            font.bold: true
            width: 300
            height: 70
            Layout.alignment: Qt.AlignHCenter

            background: Rectangle {
                radius: 20
                color: "#3498db"
            }

            onClicked: {
                console.log("Settings applied: Intensity=" + intensitySlider.value)
            }
        }
    }
}
