import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "DataManager.js" as DataManager

Item {
    width: parent.width
    height: parent.height

    property string scaleType: "linear"
    property color lineColor: "red"
    property var dataPoints: DataManager.getAmplificationData()
    property bool isSaved: false

    Column {
        anchors.fill: parent
        spacing: 10
        padding: 10

        Row {
            spacing: 20
            anchors.horizontalCenter: parent.horizontalCenter

            ComboBox {
                id: scaleSelector
                model: ["Linear", "Log"]
                currentIndex: 0
                onCurrentTextChanged: {
                    scaleType = currentText.toLowerCase()
                    chartCanvas.requestPaint()
                }
            }

            ComboBox {
                id: colorSelector
                model: ["Red", "Blue", "Green", "Orange", "Purple"]
                currentIndex: 0
                onCurrentTextChanged: {
                    switch(currentText) {
                        case "Red": lineColor = "red"; break;
                        case "Blue": lineColor = "blue"; break;
                        case "Green": lineColor = "green"; break;
                        case "Orange": lineColor = "orange"; break;
                        case "Purple": lineColor = "purple"; break;
                    }
                    chartCanvas.requestPaint()
                }
            }
        }

        Canvas {
            id: chartCanvas
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            height: parent.height - 100
            anchors.margins: 20

            onPaint: {
                var ctx = getContext("2d");
                ctx.reset();

                var left = 120, top = 80, right = width-60, bottom = height-60;

                ctx.fillStyle = "#ffffff";
                ctx.fillRect(0, 0, width, height);

                // Axes
                ctx.strokeStyle = "#000000";
                ctx.lineWidth = 3;
                ctx.beginPath();
                ctx.moveTo(left, top);
                ctx.lineTo(left, bottom);
                ctx.lineTo(right, bottom);
                ctx.stroke();

                // Tick X
                ctx.font = "32px Sans";
                ctx.fillStyle = "#000000";
                ctx.textAlign = "center";
                for(var i=0; i<=40; i+=5){
                    var px = left + (i/40)*(right-left);
                    var py = bottom;
                    ctx.beginPath();
                    ctx.moveTo(px, py);
                    ctx.lineTo(px, py+12);
                    ctx.stroke();
                    ctx.fillText(i.toString(), px, py+35);
                }

                // Tick Y
                ctx.textAlign = "right";
                ctx.font = "28px Sans";
                var yMax = Math.max.apply(Math, dataPoints.map(function(o){ return o.y; }));
                var tickCount = 8;

                if (scaleType === "log") {
                    var logMax = Math.log10(yMax + 1);
                    for (var j = 0; j <= tickCount; j++) {
                        var py = bottom - (j/tickCount) * (bottom - top);
                        var yVal = Math.pow(10, (j/tickCount) * logMax) - 1;
                        ctx.beginPath();
                        ctx.moveTo(left, py);
                        ctx.lineTo(left - 12, py);
                        ctx.stroke();
                        ctx.fillText(yVal.toFixed(1).toString(), left - 20, py + 8);
                    }
                } else {
                    var step = yMax / tickCount;
                    for (var j = 0; j <= tickCount; j++) {
                        var yVal = step * j;
                        var py = bottom - (yVal / yMax) * (bottom - top);
                        ctx.beginPath();
                        ctx.moveTo(left, py);
                        ctx.lineTo(left - 12, py);
                        ctx.stroke();
                        ctx.fillText(yVal.toFixed(1).toString(), left - 20, py + 8);
                    }
                }

                ctx.font = "50px Sans";
                ctx.textAlign = "center";
                ctx.fillText("Cycle Number", (left+right)/2, height-10);
                ctx.save();
                ctx.translate(50, (top+bottom)/2);
                ctx.rotate(-Math.PI/2);
                ctx.fillText("Fluorescence Intensity", 0, 0);
                ctx.restore();

                ctx.strokeStyle = lineColor;
                ctx.lineWidth = 4;
                ctx.beginPath();
                for(var i=0;i<dataPoints.length;i++){
                    var px = left + (dataPoints[i].x/40)*(right-left);
                    var py;
                    if(scaleType === "log") {
                        var logMax2 = Math.log10(yMax+1);
                        py = bottom - Math.log10(dataPoints[i].y+1)/logMax2*(bottom-top);
                    } else {
                        py = bottom - (dataPoints[i].y/yMax)*(bottom-top);
                    }
                    if(i===0) ctx.moveTo(px, py); else ctx.lineTo(px, py);
                }
                ctx.stroke();
            }

            Component.onCompleted: requestPaint()
        }

        Button {
            id: saveButton
            anchors.horizontalCenter: parent.horizontalCenter
            width: 220
            height: 55

            contentItem: Row {
                spacing: 10
                anchors.centerIn: parent

                Text {
                    text: "\u{1F4BE}"
                    font.pixelSize: 26
                    color: "white"
                }

                Text {
                    text: isSaved ? "Saved!" : "Save Plot"
                    font.pixelSize: 24
                    font.family: "Verdana"
                    font.bold: true
                    color: isSaved ? "#E8F5E9" : "#E3F2FD"
                }
            }

            background: Rectangle {
                radius: 15
                color: isSaved ? "#43A047" : "#1565C0"
                border.color: "#0D47A1"
                border.width: 2
            }

            onClicked: {
                isSaved = true
                chartCanvas.save("plot.png")
            }
        }
    }
}

