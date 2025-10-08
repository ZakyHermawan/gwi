import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

ColumnLayout {
    property int sliderValue:  0
    Slider {
        id: mySlider
        onValueChanged: {
            // the value variable is coming from Main.qml
            sliderValue = parseInt(value * 100)
            sliderHandler.changeSliderValue(sliderValue)
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
}
