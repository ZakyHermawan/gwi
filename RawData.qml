import QtQuick
import QtQuick.Layouts

ColumnLayout {
    Layout.fillWidth: true
    Layout.fillHeight: false

    TableView {
        Layout.fillWidth: true

        implicitHeight: 420
        implicitWidth: 500

        columnSpacing: 150
        rowSpacing: 1
        clip: true

        model: rawDataModel

        delegate: Rectangle {
            implicitWidth: 100
            implicitHeight: 50

            Text {
                font.pointSize: 24
                text: displays
            }
        }
    }
}
