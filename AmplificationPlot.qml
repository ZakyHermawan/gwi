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
            id: chart
            anchors.fill: parent
            antialiasing: true


            ValueAxis {
                id: axisX
                titleText: "Cycle"

                tickCount: {
                    if(dataManager) {
                        var numData = dataManager.maxCycle
                        if(numData > 10) return numData / 2
                        return numData
                    }
                    return 0
                }

                labelFormat: "%d"
                min: 1
                max: {
                    if(dataManager) {
                        return dataManager.maxCycle
                    }
                    return 30
                }
            }

            ValueAxis {
                id: axisY
                titleText: "Flourescence Intensity"
                min: 0
            }

            SplineSeries {
                id: lineSeries
                name: "intensity"
                axisX: axisX
                axisY: axisY
            }

            // Mouse Handling for Pan & Wheel Zoom
            MouseArea {
                anchors.fill: parent
                // Right Button for Pan, Left Button passed through to ChartView for RubberBand
                acceptedButtons: Qt.RightButton | Qt.LeftButton
                hoverEnabled: true

                property point lastPos

                onPressed: (mouse) => {
                    if (mouse.button === Qt.RightButton) {
                        // Start Panning
                        lastPos = Qt.point(mouse.x, mouse.y)
                    } else {
                        // Let ChartView handle Left Click (Rubber Band)
                        mouse.accepted = false
                    }
                }

                onPositionChanged: (mouse) => {
                    if (mouse.buttons & Qt.RightButton) {
                        // Calculate movement
                        var dx = mouse.x - lastPos.x
                        var dy = mouse.y - lastPos.y

                        // Scroll the chart
                        chart.scrollLeft(dx)
                        chart.scrollUp(dy)

                        lastPos = Qt.point(mouse.x, mouse.y)
                    }
                }

                onWheel: (wheel) => {
                    // Zoom In/Out based on scroll direction
                    if (wheel.angleDelta.y > 0) {
                        chart.zoomIn()
                    } else {
                        chart.zoomOut()
                    }
                }

                onDoubleClicked: {
                    // Reset to original view
                    chart.zoomReset()
                }
            }
        }

        VXYModelMapper {
            id: modelMapper
            model: rawDataModel
            series: lineSeries
            xColumn: 0
            yColumn: 1
            firstRow: 1
        }
    }
}
