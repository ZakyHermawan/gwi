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
                tickCount: {
                    if(dataManager) {
                        var numData = dataManager.getIntensityValuesSize()
                        if(numData > 10) {
                            return numData / 2
                        }
                        return numData
                    }
                    return 0
                }

                labelFormat: "%d"

                min: 1
                max: {
                    if(dataManager) {
                        return dataManager.getIntensityValuesSize()
                    }
                    return 1
                }
            }

            ValueAxis {
                id: axisY
                titleText: "Fluorescence Intensity"
                min: 0
            }

            SplineSeries {
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

            // The model starts plotting from row 1 (index 1)
            firstRow: 1
        }
    }
}
