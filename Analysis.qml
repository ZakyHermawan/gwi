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
                text: {
                    if(dataManager) {
                        return dataManager.rSquared.toFixed(3)
                    }
                    return ""
                }

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
                text: {
                    if(dataManager) {
                        return dataManager.yIntercept.toFixed(3)
                    }
                    return ""
                }

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
                text: {
                    if(dataManager) {
                        return dataManager.slope.toFixed(3)
                    }
                    return ""
                }

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
                text: {
                    if(dataManager) {
                        return String(dataManager.percentEfficiency.toFixed(2)) + "%"
                    }
                    return ""
                }

                font.pointSize: 24
            }
        }
    }

}
