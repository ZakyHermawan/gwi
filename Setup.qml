import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import QtQuick.VirtualKeyboard

ColumnLayout {
    property int sliderValue:  0

    Slider {
        id: mySlider
        value: {
            if(typeof sliderHandler !== "undefined" && sliderHandler) {
                // slider value is a real number from 0 to 1
                return sliderHandler.getCurrentValue() / 100
            } else {
                return 0
            }
        }

        onValueChanged: {
            sliderValue = parseInt(value * 100)
            if(typeof sliderHandler !== "undefined" && sliderHandler) {
                sliderHandler.changeSliderValue(sliderValue)
            }
        }
        implicitWidth: 300
        implicitHeight: 50
        leftPadding: 30
    }

    Text {
        text: "Light Intensity Level: " + sliderValue + " %"
        font.pointSize: 24
        leftPadding: 30
    }

    Rectangle {
        Layout.topMargin: 10
        Layout.fillWidth: true

        ColumnLayout {
            id: setupLayout
            property bool blockInputFromNumberofCycle: false
            property bool blockInputFromConcentrationCoefficient: false

            function blockButtonsAndShowWarningIfNecessary() {
                if(setupLayout.blockInputFromNumberofCycle || setupLayout.blockInputFromConcentrationCoefficient) {
                    window.inputBlocked = true
                    window.blockRun = true

                    if(setupLayout.blockInputFromNumberofCycle) {
                        cycleNumberInputWarningMessage.text = "Cycle number must be integer > 0"
                    } else {
                        cycleNumberInputWarningMessage.text = ""
                    }

                    if(setupLayout.blockInputFromConcentrationCoefficient) {
                        concentrationInputWarningMessage.text = "Concentration coefficient\nmust be a valid non-zero floating point number"
                    } else {
                        concentrationInputWarningMessage.text = ""
                    }
                } else {
                    window.inputBlocked = false
                    window.blockRun = false
                    cycleNumberInputWarningMessage.text = ""
                    concentrationInputWarningMessage.text = ""
                }
            }

            RowLayout {
                Text {
                    text: "Maximum number of cycle: "
                    font.pointSize: 24
                    leftPadding: 30
                }

                TextInput {
                    id: cycleInput
                    Layout.minimumWidth: 20
                    font.pointSize: 24
                    text: {
                        if(typeof dataManager !== "undefined" && dataManager) {
                            return String(dataManager.getIntensityValuesSize())
                        }
                        return "30"
                    }

                    inputMethodHints: Qt.ImhDigitsOnly
                    onEditingFinished: {
                        focus = false
                    }

                    validator: IntValidator {
                        bottom: 1
                        top: 2147483647
                    }

                    onTextChanged: {
                        if (cycleInput.acceptableInput) {
                            var val = parseInt(text)
                            setupLayout.blockInputFromNumberofCycle = false
                            if(typeof dataManager !== "undefined" && dataManager) {
                                dataManager.setIntensityValuesSize(val)
                            }
                        } else {
                            setupLayout.blockInputFromNumberofCycle = true
                        }
                        setupLayout.blockButtonsAndShowWarningIfNecessary()
                    }
                }
            }

            RowLayout {
                Text {
                    text: "Concentration: "
                    font.pointSize: 24
                    leftPadding: 30
                }

                TextInput {
                    id: concentrationCoefficientInput
                    Layout.minimumWidth: 20
                    font.pointSize: 24
                    text: {
                        if(typeof dataManager !== "undefined" && dataManager) {
                            return dataManager.concentrationCoefficient
                        }
                        return "1"
                    }
                    inputMethodHints: Qt.ImhDigitsOnly
                    onEditingFinished: {
                        focus = false
                    }
                    onTextChanged: {
                        var strictFloatRegex = /^-?(\d+\.?\d*|\.\d+)$/;

                        if (strictFloatRegex.test(text)) {
                            var val = parseFloat(text);
                            if (val === 0) {
                                setupLayout.blockInputFromConcentrationCoefficient = true
                            } else {
                                setupLayout.blockInputFromConcentrationCoefficient = false
                            }
                        } else {
                            setupLayout.blockInputFromConcentrationCoefficient = true
                        }

                        setupLayout.blockButtonsAndShowWarningIfNecessary()

                        if(typeof dataManager !== "undefined" && dataManager) {
                            dataManager.concentrationCoefficient = text
                        }
                    }
                }

                Text {
                    text: "x"
                    font.pointSize: 24
                }

                ComboBox {
                    id: concentrationMultiplierInput
                    Layout.preferredWidth: 175
                    Layout.minimumWidth: 100
                    font.pointSize: 14

                    model: [
                        "Tera (T)  10¹²",
                        "Giga (G)  10⁹",
                        "Mega (M)  10⁶",
                        "Kilo (k)  10³",
                        "Base Unit 10⁰",
                        "Milli (m) 10⁻³",
                        "Micro (μ) 10⁻⁶",
                        "Nano (n)  10⁻⁹",
                        "Pico (p)  10⁻¹²"
                    ]

                    property var multipliers: [
                        1e12,  // Tera
                        1e9,   // Giga
                        1e6,   // Mega
                        1000,  // Kilo
                        1.0,   // Base
                        0.001, // Milli
                        1e-6,  // Micro
                        1e-9,  // Nano
                        1e-12  // Pico
                    ]

                    currentIndex: {
                        if (typeof dataManager !== "undefined" && dataManager) {
                            var multiplier = dataManager.getConcentrationMultiplier()
                            for (var i = 0; i < multipliers.length; i++) {
                                var target = multipliers[i]
                                var diff = Math.abs(multiplier - target)
                                if (diff <= (target * 0.01)) {
                                    return i
                                }
                            }
                        }
                        return 4
                    }

                    onCurrentIndexChanged: {
                        var multiplier = multipliers[currentIndex]
                        if (typeof dataManager !== "undefined" && dataManager) {
                            dataManager.setConcentrationMultiplier(multiplier)
                        }
                    }
                }
            }

            Button {
                text: "Reset data"
                font.pointSize: 24
                Layout.leftMargin: 30
                palette.button: "orange"

                onClicked: {
                    if(typeof sliderHandler !== "undefined" && sliderHandler) {
                        mySlider.value = 0
                        cycleInput.text = "30"
                        concentrationCoefficientInput.text = "1"
                        concentrationMultiplierInput.currentIndex = 4
                    }
                    if(typeof dataManager !== "undefined" && dataManager) {
                        dataManager.resetIntensityValues()
                        dataManager.resetStandardCurveData()
                    }
                }
            }

            Text {
                id: cycleNumberInputWarningMessage
                text: ""
                color: "red"
                font.pointSize: 24
                leftPadding: 30
                visible: text !== ""
                Layout.topMargin: visible ? 10 : 0
            }

            Text {
                id: concentrationInputWarningMessage
                text: ""
                color: "red"
                font.pointSize: 24
                leftPadding: 30
                visible: text !== ""
                Layout.topMargin: visible ? 10 : 0
            }

            InputPanel {
                id: inputPanel
                z: 99

                // Inherit directly from root/main window, so the keyboard is overlays the whole screen
                parent: window.contentItem

                // Anchor to the window edges
                anchors.left: parent.left
                anchors.right: parent.right

                // Initial position (hidden below screen)
                y: window.height

                states: State {
                    name: "visible"
                    when: inputPanel.active
                    PropertyChanges {
                        target: inputPanel
                        // Position at the bottom of the window
                        // Give some offset, so it does not cover the concentration coefficient input
                        y: window.height - inputPanel.height + 10
                    }
                }

                transitions: Transition {
                    from: ""
                    to: "visible"
                    reversible: true
                    ParallelAnimation {
                        NumberAnimation {
                            properties: "y"
                            duration: 250
                            easing.type: Easing.InOutQuad
                        }
                    }
                }
            }
        }
    }
}
