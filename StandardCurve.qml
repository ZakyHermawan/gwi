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
            id: chart
            anchors.fill: parent
            antialiasing: true

            ValueAxis {
                id: axisX
                titleText: "Log Starting Quantity"
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
                markerSize: 10 // Make points easier to see
            }

            // Mouse Handling for Wheel Zoom & Panning
            MouseArea {
                anchors.fill: parent
                // Accept Right for Pan, Left is passed through for Rubber Band
                acceptedButtons: Qt.RightButton | Qt.LeftButton
                hoverEnabled: true

                property point lastPos

                onPressed: (mouse) => {
                    if (mouse.button === Qt.RightButton) {
                        lastPos = Qt.point(mouse.x, mouse.y)
                    } else {
                        // Pass Left Click to ChartView for Rubber Band selection
                        mouse.accepted = false
                    }
                }

                onPositionChanged: (mouse) => {
                    if (mouse.buttons & Qt.RightButton) {
                        // Pan Logic
                        var dx = mouse.x - lastPos.x
                        var dy = mouse.y - lastPos.y

                        // "scrollLeft" moves the view, so positive dx moves view left
                        chart.scrollLeft(dx)
                        chart.scrollUp(dy)

                        lastPos = Qt.point(mouse.x, mouse.y)
                    }
                }

                onWheel: (wheel) => {
                    // Zoom In/Out
                    if (wheel.angleDelta.y > 0) {
                        chart.zoomIn()
                    } else {
                        chart.zoomOut()
                    }
                }

                onDoubleClicked: {
                    // Reset View
                    chart.zoomReset()
                }
            }
        }

        VXYModelMapper {
            id: modelMapper

            model: standardCurveModel
            series: lineSeries
            xColumn: 0
            yColumn: 1
            firstRow: 0
        }
    }
}
