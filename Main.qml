import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtCharts 2.15

Window {
    id: window
    width: 1024
    height: 600
    visible: true
    title: qsTr("Menu Awal")
    Button {
        text: "Minimize"
        onClicked: {
            window.visibility = Window.Minimized
        }
    }
    property string latestButton: "Setup"
    property string sourceFileName: "IntensitySlider.qml"
    property string globalFont: "Segoe UI"

    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#83edd2" }
            GradientStop { position: 1.0; color: "#83b7ed" }
        }
    }


    function updateButton(currentButton) {
        setupButton.background.color = "#bdc3c7"
        runButton.background.color = "#bdc3c7"
        amplificationPlotButton.background.color = "#bdc3c7"
        standardCurveButton.background.color = "#bdc3c7"
        rawDataButton.background.color = "#bdc3c7"
        summaryButton.background.color = "#bdc3c7"
        endButton.background.color = "#bdc3c7"

        if (currentButton === "Setup") {
            setupButton.background.color = "#f1c40f"
            sourceFileName = "SetupPage.qml"
        } else if (currentButton === "Run") {
            runButton.background.color = "#3498db"
            sourceFileName = "Run.qml"
        } else if (currentButton === "Amplification Plot") {
            amplificationPlotButton.background.color = "#1abc9c"
            sourceFileName = "AmplificationPlot.qml"
        } else if (currentButton === "Standard Curve") {
            standardCurveButton.background.color = "#e67e22"
            sourceFileName = "StandardCurve.qml"
        } else if (currentButton === "Raw Data") {
            rawDataButton.background.color = "#2ecc71"
            sourceFileName = "RawData.qml"
        } else if (currentButton === "Summary") {
            summaryButton.background.color = "#2630e6"
            sourceFileName = "Summary.qml"
        } else {
            endButton.background.color = "#e74c3c"
            sourceFileName = "END.qml"
        }

        latestButton = currentButton
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 15
        spacing: 25

        ColumnLayout {
            Layout.alignment: Qt.AlignTop
            spacing: 15
            Rectangle {
                Layout.preferredWidth: 300
                Layout.preferredHeight: 100
                radius: 15
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#3498db" }
                    GradientStop { position: 1.0; color: "#2980b9" }
                }
                Text {
                    anchors.centerIn: parent
                    text: "📋 Menu"
                    font.family: globalFont
                    font.pixelSize: 36
                    font.bold: true
                    color: "white"
                }
            }

            Button {
                id: setupButton
                width: 300; height: 80
                text: "Setup"
                font.family: globalFont
                font.pixelSize: 34
                font.bold: true
                background: Rectangle { radius: 15; color: "#f1c40f" }
                onClicked: updateButton("Setup")
            }
            Button {
                id: runButton
                width: 300; height: 80
                text: "Run"
                font.family: globalFont
                font.pixelSize: 34
                font.bold: true
                background: Rectangle { radius: 15; color: "#bdc3c7" }
                onClicked: updateButton("Run")
            }
            Button {
                id: amplificationPlotButton
                width: 300; height: 80
                text: "Amplification Plot"
                font.family: globalFont
                font.pixelSize: 34
                font.bold: true
                background: Rectangle { radius: 15; color: "#bdc3c7" }
                onClicked: updateButton("Amplification Plot")
            }
            Button {
                id: standardCurveButton
                width: 300; height: 80
                text: "Standard Curve"
                font.family: globalFont
                font.pixelSize: 34
                font.bold: true
                background: Rectangle { radius: 15; color: "#bdc3c7" }
                onClicked: updateButton("Standard Curve")
            }
            Button {
                id: rawDataButton
                width: 300; height: 80
                text: "Raw Data"
                font.family: globalFont
                font.pixelSize: 34
                font.bold: true
                background: Rectangle { radius: 15; color: "#bdc3c7" }
                onClicked: updateButton("Raw Data")
            }
            Button {
                id: summaryButton
                width: 300; height: 80
                text: "Summary"
                font.family: globalFont
                font.pixelSize: 34
                font.bold: true
                background: Rectangle { radius: 15; color: "#bdc3c7" }
                onClicked: updateButton("Summary")
            }
            Button {
                id: endButton
                width: 300; height: 80
                text: "End"
                font.family: globalFont
                font.pixelSize: 34
                font.bold: true
                background: Rectangle { radius: 15; color: "#bdc3c7" }
                onClicked: updateButton("End")
            }
        }


        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 20

            Rectangle {
                Layout.fillWidth: true
                height: 90
                radius: 20
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#ecf0f1" }
                    GradientStop { position: 1.0; color: "#bdc3c7" }
                }
                border.color: "#95a5a6"

                Text {
                    anchors.centerIn: parent
                    text: "🔬 Experiment"
                    font.family: globalFont
                    font.pixelSize: 36
                    font.bold: true
                    color: "#2c3e50"
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                radius: 22
                color: "white"
                border.color: "#dcdde1"

                Loader {
                    id: mainLoader
                    anchors.fill: parent
                    anchors.margins: 15
                    source: sourceFileName
                }


            }
        }
    }
}
