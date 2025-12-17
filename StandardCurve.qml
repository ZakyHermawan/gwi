import QtQuick
import QtCharts
import QtQuick.Layouts

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
                titleText: "Log Starting Quantity"
                tickCount: {
                    if(dataManager) {
                        var numData = dataManager.getStandardCurveDataSize()
                        return numData
                    }
                    return 0
                }

                labelFormat: "%.2f"
            }

            ValueAxis {
                id: axisY
                titleText: "Cq"
            }

            ScatterSeries {
                id: lineSeries
                name: "Standard Curve"

                axisX: axisX
                axisY: axisY
            }
        }

        VXYModelMapper {
            id: modelMapper

            model: standardCurveModel
            series: lineSeries
            xColumn: 0
            yColumn: 1

            // The model starts plotting from row 1 (index 1)
            firstRow: 0
        }
    }
}
