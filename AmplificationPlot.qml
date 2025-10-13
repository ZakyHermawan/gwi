import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

import QtCharts

ColumnLayout {

    Rectangle {
        height: 100
        color: "red"

        border.width: 3
        border.color: "black"

        Layout.preferredWidth: 700
    }

    Rectangle {
        Layout.preferredHeight: 340
        Layout.preferredWidth: 700

        border.width: 3
        border.color: "black"

        ChartView {
            anchors.fill: parent
            antialiasing: true

            ValueAxis {
                id: axisX
                titleText: "Cycle"
                tickCount: 16
                labelFormat: "%d"

                min: 0
                max: 30
            }

            ValueAxis {
                id: axisY
                titleText: "Fluorescence Intensity"

                min: 0
            }

            LineSeries {
                id: lineSeries
                name: "intensity"

                axisX: axisX
                axisY: axisY
            }

        }

        VXYModelMapper {
            id: modelMapper

            model: rawDataModel
            series: lineSeries
            xColumn: 0
            yColumn: 1
        }

    }
}
