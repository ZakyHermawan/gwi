import QtQuick
import QtQuick.Layouts

Rectangle {
    ColumnLayout {
        RowLayout {
            Text {
                text: "R² = "
                font.pointSize: 24
                leftPadding: 30
            }

            Text {
                text: "0.999"
                font.pointSize: 24
            }
        }

        RowLayout {
            Text {
                text: "Y-intercept = "
                font.pointSize: 24
                leftPadding: 30
            }

            Text {
                text: "20.318"
                font.pointSize: 24
            }
        }

        RowLayout {
            Text {
                text: "Slope = "
                font.pointSize: 24
                leftPadding: 30
            }

            Text {
                text: "-3.258"
                font.pointSize: 24
            }
        }

        RowLayout {
            Text {
                text: "Efficiency = "
                font.pointSize: 24
                leftPadding: 30
            }

            Text {
                text: "102.8%"
                font.pointSize: 24
            }
        }
    }

}
