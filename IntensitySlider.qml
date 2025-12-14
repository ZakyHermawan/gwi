import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

ColumnLayout {
    property int sliderValue:  0
    Slider {
        id: mySlider
        value: {
            if(sliderHandler) {
                // slider value is a real number from 0 to 1
                return sliderHandler.getCurrentValue() / 100
            }
            else
            {
                return 0
            }
        }

        onValueChanged: {
            sliderValue = parseInt(value * 100)
            if(sliderHandler) {
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
                    }
                    else {
                        cycleNumberInputWarningMessage.text = ""
                    }

                    if(setupLayout.blockInputFromConcentrationCoefficient) {
                        concentrationInputWarningMessage.text = "Concentration coefficient\nmust be a valid floating point number"
                    }
                    else {
                        concentrationInputWarningMessage.text = ""
                    }
                }
                else {
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

                    text: {
                        if(dataManager) {
                            return String(dataManager.getIntensityValuesSize())
                        }
                        return "30"
                    }

                    font.pointSize: 24

                    // PRIMARY PROTECTION: Only allow integers > 0
                    // This stops the user from typing "abc", "-", "/", or left the input as empty
                    validator: IntValidator {
                        bottom: 1
                        top: 2147483647 // Max 32-bit int to prevent overflow
                    }

                    // SECONDARY PROTECTION: Sanitize before sending to C++
                    onTextChanged: {
                        // Check if input is acceptable (passes the validator)
                        if (cycleInput.acceptableInput) {
                            var val = parseInt(text)
                            setupLayout.blockInputFromNumberofCycle = false

                            if(dataManager) {
                                // Update the number of cycle
                                dataManager.setIntensityValuesSize(val)
                            }
                        }
                        else {
                            // Invalid input format
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
                    // Coefficient
                    Layout.minimumWidth: 20
                    text: {
                        if (dataManager) {
                            return dataManager.concentrationCoefficient
                        }
                        return "1"
                    }

                    font.pointSize: 24
                    onTextChanged: {
                        // Regex: Optional minus, then either (digits + optional dot) OR (dot + digits).
                        // Matches: "12", "12.5", "12.", ".5", "-.5"
                        // Rejects: ".", "-", "-." (Ensures at least one digit exists)
                        var strictFloatRegex = /^-?(\d+\.?\d*|\.\d+)$/;

                        if (strictFloatRegex.test(text)) {
                            setupLayout.blockInputFromConcentrationCoefficient = false
                        }
                        else {
                            setupLayout.blockInputFromConcentrationCoefficient = true
                        }

                        setupLayout.blockButtonsAndShowWarningIfNecessary()

                        // Assign the raw concentration string; sanitization is handled downstream.
                        if(dataManager) {
                            // text is a valid floating point literal
                            // But we still need to sanitize in case of something like "2.", ".2", or ".0"
                            // Make sure we handle these cases correctly.
                            // Also consider cases like 00123.6, 0000123, or 00
                            dataManager.concentrationCoefficient = text
                        }
                    }
                }

                Text {
                    text: "x"
                    font.pointSize: 24
                }

                ComboBox {
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

                            // Loop through all multipliers to find the matching index for the multiplier
                            for (var i = 0; i < multipliers.length; i++) {
                                var target = multipliers[i]

                                // Floating Point Comparison Logic
                                // We check if the difference is very small relative to the target value.
                                // (target * 0.01) means we allow a 1% margin of error, which covers
                                // float precision issues for both Tera (1e12) and Pico (1e-12).
                                var diff = Math.abs(multiplier - target)

                                if (diff <= (target * 0.01)) {
                                    return i // Found the matching index!
                                }
                            }
                        }

                        // Fallback/Default
                        return 4 // Return index 4 (Base 1.0) if nothing matches
                    }

                    onCurrentIndexChanged: {
                        var multiplier = multipliers[currentIndex]
                        if (dataManager) {
                            dataManager.setConcentrationMultiplier(multiplier)
                        }
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
        }
    }
}
