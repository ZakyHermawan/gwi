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
        RowLayout {
            Text {
                text: "Maximum number of cycle: "
                font.pointSize: 24
                leftPadding: 30
            }

            TextEdit {
                text: {
                    if(dataManager) {
                        dataManager.getIntensityValuesSize()
                    }
                }

                font.pointSize: 24
                onTextChanged: dataManager.setIntensityValuesSize(text)
            }
        }
    }
}
