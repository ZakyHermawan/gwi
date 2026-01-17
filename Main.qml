import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.VirtualKeyboard

Window {
    id: window
    width: 1024
    height: 600
    visible: true
    title: qsTr("PCR Graphical User Interface")

    property bool inputBlocked: false
    property bool blockRun: false
    property string latestButton: "Setup"
    property string sourceFileName: "Setup.qml"

    function updateButton(currentButton) {
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
            setupButton.palette.button = "Blue"
        }
        else if(currentButton === "Run") {
            runButton.palette.button = "Blue"
        }
        else if(currentButton === "Analysis") {
            analysisButton.palette.button = "Blue"
        }
        else if(currentButton === "Amplification Plot") {
            amplificationPlotButton.palette.button = "Blue"
        }
        else if(currentButton === "Standard Curve") {
            standardCurveButton.palette.button = "Blue"
        }
        else if(currentButton === "Raw Data") {
            rawDataButton.palette.button = "Blue"
        }
        else if(currentButton === "Summary") {
            summaryButton.palette.button = "Blue"
        }
        else {
            endButton.palette.button = "Blue"
        }

        latestButton = currentButton

        if(currentButton === "Setup") {
            sourceFileName = "Setup.qml"
        }
        else if(currentButton === "Amplification Plot") {
            sourceFileName = "AmplificationPlot.qml"
        }
        else if(currentButton === "Analysis") {
            sourceFileName = "Analysis.qml"
        }
        else if(currentButton === "Standard Curve") {
            sourceFileName = "StandardCurve.qml"
        }
        else if(currentButton === "Raw Data") {
            sourceFileName = "RawData.qml"
        }
        else if(currentButton === "Summary") {
            sourceFileName = "Summary.qml"
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
                Layout.preferredHeight: 70
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
                Layout.preferredHeight: 70
                Button {
                    id: setupButton
                    width: 250
                    height: 70
                    text: "Setup"
                    font.pixelSize: 30
                    palette.button: "Blue"
                    enabled: !window.inputBlocked

                    onClicked: {
                        buttonHandler.handleButtonClick(text);
                        stateManager.changeCurrentState(text);
                        updateButton("Setup")
                    }
                }
            }

            Rectangle {
                Layout.alignment: Qt.AlignLeft
                Layout.preferredHeight: 70
                Button {
                    id: runButton
                    width: 250
                    height: 70
                    text: "Run"
                    objectName: "runButton"
                    font.pixelSize: 30
                    palette.button: "red"
                    enabled: !window.blockRun

                    onClicked: {
                        // Event filter (in RunButtonEventFilter.cpp)
                        // being executed before onClicked.
                        // Therefore, the value of runButton.text is changed already

                        if(dataManager && text === "Run") {
                            // If the current text is "Run", it means that
                            // the text before this button were clicked is "Stop"
                            dataManager.calculateStandardCurve()
                        }

                        buttonHandler.handleButtonClick(text);
                        stateManager.changeCurrentState(text);
                        window.inputBlocked = !window.inputBlocked;
                        updateButton("Run")
                    }
                }
            }

            Rectangle {
                Layout.alignment: Qt.AlignLeft
                Layout.preferredHeight: 70
                Button {
                    id: amplificationPlotButton
                    width: 250
                    height: 70
                    text: "Amplification Plot"
                    font.pixelSize: 30
                    palette.button: "red"
                    enabled: !window.inputBlocked

                    onClicked: {
                        buttonHandler.handleButtonClick(text);
                        stateManager.changeCurrentState(text);
                        updateButton("Amplification Plot")
                    }
                }
            }

            Rectangle {
                Layout.alignment: Qt.AlignLeft
                Layout.preferredHeight: 70
                Button {
                    id: rawDataButton
                    width: 250
                    height: 70
                    text: "Raw Data"
                    font.pixelSize: 30
                    palette.button: "red"
                    enabled: !window.inputBlocked

                    onClicked: {
                        buttonHandler.handleButtonClick(text);
                        stateManager.changeCurrentState(text);
                        updateButton("Raw Data")
                    }
                }
            }

            Rectangle {
                Layout.alignment: Qt.AlignLeft
                Layout.preferredHeight: 70
                Button {
                    id: standardCurveButton
                    width: 250
                    height: 70
                    text: "Standard Curve"
                    font.pixelSize: 30
                    palette.button: "red"
                    enabled: !window.inputBlocked

                    onClicked: {
                        buttonHandler.handleButtonClick(text);
                        stateManager.changeCurrentState(text);
                        updateButton("Standard Curve")
                    }
                }
            }

            Rectangle {
                Layout.alignment: Qt.AlignLeft
                Layout.preferredHeight: 70
                Button {
                    id: analysisButton
                    width: 250
                    height: 70
                    text: "Analysis"
                    font.pixelSize: 30
                    palette.button: "red"
                    enabled: !window.inputBlocked

                    onClicked: {
                        buttonHandler.handleButtonClick(text);
                        stateManager.changeCurrentState(text);
                        updateButton("Analysis")
                    }
                }
            }

            Rectangle {
                Layout.alignment: Qt.AlignLeft
                Layout.preferredHeight: 70
                Button {
                    id: summaryButton
                    width: 250
                    height: 70
                    text: "Summary"
                    font.pixelSize: 30
                    palette.button: "red"
                    enabled: !window.inputBlocked

                    onClicked: {
                        buttonHandler.handleButtonClick(text);
                        stateManager.changeCurrentState(text);
                        updateButton("Summary")
                    }
                }
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 10

            Rectangle {
                Layout.alignment: Qt.AlignLeft
                Layout.preferredHeight: 80

                // Ensure the main wrapper expands to fit both the ComboBox
                Layout.preferredWidth: contentRow.width
                color: "transparent"

                Row {
                    id: contentRow
                    spacing: 10

                    ComboBox {
                        id: myComboBox
                        height: 50
                        font.pixelSize: 30
                        implicitWidth: 350

                        model: experimentModel
                        editable: true
                        textRole: "experimentName"
                        enabled: !window.inputBlocked

                        onActivated: (index) => {
                            if (index >= 0) {
                                experimentModel.loadExperiment(index)
                            }
                        }

                        onAccepted: {
                            focus = false // Remove focus to hide keyboard/cursor

                            // Make sure the file name as .yml extension
                            var cleanText = editText.trim()
                            if (cleanText === "") return
                            if (!cleanText.endsWith(".yml")) {
                                cleanText += ".yml"
                            }
                            experimentModel.addEntry(cleanText)
                        }

                        Component.onCompleted: {
                            if (count > 0) {
                                currentIndex = 0
                                experimentModel.loadExperiment(0)
                            }
                        }
                    }
                    Rectangle {
                        id: deleteBtn
                        width: deleteLabel.implicitWidth + 40
                        height: 50
                        enabled: !window.inputBlocked

                        color: deleteMouseArea.pressed ? "#cc0000" : "#ff5555"
                        border.color: "#555555"
                        radius: 2

                        Text {
                            id: deleteLabel
                            anchors.centerIn: parent
                            text: "Delete current experiment"

                            font.pixelSize: 20
                            font.bold: true
                            color: "white"
                        }

                        MouseArea {
                            id: deleteMouseArea
                            anchors.fill: parent

                            onClicked: {
                                var idx = myComboBox.currentIndex
                                if (idx >= 0) {
                                    // Remove (and Auto-Add if last item)
                                    experimentModel.removeEntry(idx)

                                    // Select the correct item
                                    // If we deleted the last item and it auto-created a new one,
                                    // count will be 1, so we select 0.
                                    if (myComboBox.count > 0) {
                                        myComboBox.currentIndex = 0
                                        experimentModel.loadExperiment(0)
                                    } else {
                                        // This block should technically never be reached
                                        // if the auto-create logic works
                                        myComboBox.currentIndex = -1
                                    }
                                }
                            }
                        }
                    }
                }
            }

            Item {
                id: itemParent
                Layout.preferredHeight: 450
                Layout.fillWidth: true
                Loader {
                    source: sourceFileName
                }
            }

            RowLayout {
                Layout.topMargin: -10
                Button {
                    id: saveDataButton
                    text: "Save Data"
                    font.pixelSize: 30
                    Layout.preferredHeight: 50
                    Layout.preferredWidth: 350
                    palette.button: "lightblue"
                    enabled: !window.inputBlocked
                    onClicked: buttonHandler.saveDataClick()
                }

                Button {
                    id: endButton
                    text: "End"
                    font.pixelSize: 30
                    Layout.leftMargin: 25
                    Layout.preferredHeight: 50
                    Layout.preferredWidth: 350
                    palette.button: "lightblue"
                    enabled: !window.inputBlocked

                    onClicked: {
                        buttonHandler.handleButtonClick(text);
                        stateManager.changeCurrentState(text);
                        updateButton("End")
                    }
                }
            }
        }
    }
}
