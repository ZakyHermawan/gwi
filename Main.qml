import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Basic

Window {
    id: window
    width: 1024
    height: 600
    visible: true
    title: qsTr("Hello World")

    property string latestButton: "Setup"
    property string sourceFileName: "IntensitySlider.qml"

    function updateButton(currentButton) {
        console.log("latest button: " + latestButton + ", Current button: " + currentButton)
        if(latestButton === "Setup") {
            setupButton.palette.button = "Red"
        }
        else if(latestButton === "Run") {
            runButton.palette.button = "Red"
        }
        else if(latestButton === "Analysis") {
            analysisButton.palette.button = "Red"
        }
        else if(latestButton === "Amplification Plot") {
            amplificationPlotButton.palette.button = "Red"
        }
        else if(latestButton === "Standard Curve") {
            standardCurveButton.palette.button = "Red"
        }
        else if(latestButton === "Raw Data") {
            rawDataButton.palette.button = "Red"
        }
        else if(latestButton === "Summary") {
            summaryButton.palette.button = "Red"
        }
        else {
            endButton.palette.button = "Red"
        }


        if(currentButton === "Setup") {
            setupButton.palette.button = "Yellow"
        }
        else if(currentButton === "Run") {
            runButton.palette.button = "Yellow"
        }
        else if(currentButton === "Analysis") {
            analysisButton.palette.button = "Yellow"
        }
        else if(currentButton === "Amplification Plot") {
            amplificationPlotButton.palette.button = "Yellow"
        }
        else if(currentButton === "Standard Curve") {
            standardCurveButton.palette.button = "Yellow"
        }
        else if(currentButton === "Raw Data") {
            rawDataButton.palette.button = "Yellow"
        }
        else if(currentButton === "Summary") {
            summaryButton.palette.button = "Yellow"
        }
        else {
            endButton.palette.button = "Yellow"
        }

        latestButton = currentButton

        if(currentButton === "Setup") {
            sourceFileName = "IntensitySlider.qml"
        }
        else if(currentButton === "Amplification Plot") {
            sourceFileName = "AmplificationPlot.qml"
        }

        else {
            sourceFileName = ""
        }
    }

    RowLayout {
        spacing: 20

        ColumnLayout {
            Layout.alignment: Qt.AlignTop
            Rectangle {
                Layout.alignment: Qt.AlignLeft
                Layout.preferredWidth: 250
                Layout.preferredHeight: 80
                Text {
                    anchors.fill: parent
                    font.pixelSize: 30
                    text: "Menu"
                    fontSizeMode: Text.Fit
                    minimumPixelSize: 10
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Rectangle {
                Layout.alignment: Qt.AlignLeft
                Layout.preferredHeight: 60
                Button {
                    id: setupButton
                    width: 250
                    text: "Setup"
                    font.pixelSize: 30
                    palette.button: "yellow"
                    onClicked: {
                        buttonHandler.handleButtonClick(text);
                        stateManager.changeCurrentState(text);
                        updateButton("Setup")
                    }
                }
            }

            Rectangle {
                Layout.alignment: Qt.AlignLeft
                Layout.preferredHeight: 60
                Button {
                    id: runButton
                    width: 250
                    text: "Run"
                    font.pixelSize: 30
                    palette.button: "red"
                    onClicked: {
                        buttonHandler.handleButtonClick(text);
                        stateManager.changeCurrentState(text);
                        updateButton("Run")
                    }
                }
            }

            Rectangle {
                Layout.alignment: Qt.AlignLeft
                Layout.preferredHeight: 60
                Button {
                    id: analysisButton
                    width: 250
                    text: "Analysis"
                    font.pixelSize: 30
                    palette.button: "red"
                    onClicked: {
                        buttonHandler.handleButtonClick(text);
                        stateManager.changeCurrentState(text);
                        updateButton("Analysis")
                    }
                }
            }

            Rectangle {
                Layout.alignment: Qt.AlignLeft
                Layout.preferredHeight: 60
                Button {
                    id: amplificationPlotButton
                    width: 250
                    text: "Amplification Plot"
                    font.pixelSize: 30
                    palette.button: "red"
                    onClicked: {
                        buttonHandler.handleButtonClick(text);
                        stateManager.changeCurrentState(text);
                        updateButton("Amplification Plot")
                    }
                }
            }

            Rectangle {
                Layout.alignment: Qt.AlignLeft
                Layout.preferredHeight: 60
                Button {
                    id: standardCurveButton
                    width: 250
                    text: "Standard Curve"
                    font.pixelSize: 30
                    palette.button: "red"
                    onClicked: {
                        buttonHandler.handleButtonClick(text);
                        stateManager.changeCurrentState(text);
                        updateButton("Standard Curve")
                    }
                }
            }

            Rectangle {
                Layout.alignment: Qt.AlignLeft
                Layout.preferredHeight: 60
                Button {
                    id: rawDataButton
                    width: 250
                    text: "Raw Data"
                    font.pixelSize: 30
                    palette.button: "red"
                    onClicked: {
                        buttonHandler.handleButtonClick(text);
                        stateManager.changeCurrentState(text);
                        updateButton("Raw Data")
                    }
                }
            }

            Rectangle {
                Layout.alignment: Qt.AlignLeft
                Layout.preferredHeight: 60
                Button {
                    id: summaryButton
                    width: 250
                    text: "Summary"
                    font.pixelSize: 30
                    palette.button: "red"
                    onClicked: {
                        buttonHandler.handleButtonClick(text);
                        stateManager.changeCurrentState(text);
                        updateButton("Summary")
                    }
                }
            }

            Rectangle {
                Layout.alignment: Qt.AlignLeft
                Layout.preferredHeight: 60
                Button {
                    id: endButton
                    width: 250
                    text: "End"
                    font.pixelSize: 30
                    palette.button: "red"
                    onClicked: {
                        buttonHandler.handleButtonClick(text);
                        stateManager.changeCurrentState(text);
                        updateButton("End")
                    }
                }
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Rectangle {
                Layout.alignment: Qt.AlignLeft
                Layout.preferredWidth: 250
                Layout.preferredHeight: 80

                Text {
                    text: "Experiment"
                    font.pixelSize: 30
                    fontSizeMode: Text.Fit
                    minimumPixelSize: 10
                    leftPadding: 30
                    topPadding: 20
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Item {
                id: itemParent
                Layout.fillHeight: true
                Layout.fillWidth: true
                Loader {
                    source: sourceFileName
                }
            }

            Button {
                text: "Save Plot"
                font.pixelSize: 30
                Layout.preferredHeight: 50
                Layout.preferredWidth: 150
                Layout.bottomMargin: 10
                palette.button: "lightblue"
                onClicked: buttonHandler.savePlotClick()
            }
        }
    }
}

