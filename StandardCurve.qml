import QtQuick 2.15
import QtQuick.Controls 2.15
import "DataManager.js" as DataManager

Item {
    width: 750
    height: 550

    property string xScaleType: "log"
    property color lineColor: "blue"
    property var standardData: DataManager.getStandardCurveData()

    Column {
        anchors.fill: parent
        spacing: 15
        anchors.margins: 15

        Row {
            id: comboRow
            spacing: 20
            width: parent.width - 30
            anchors.horizontalCenter: parent.horizontalCenter

            ComboBox {
                id: scaleSelector
                width: 180
                model: ["Linear", "Log"]
                currentIndex: 0
                onCurrentTextChanged: {
                    xScaleType = currentText.toLowerCase()
                    chartCanvas.requestPaint()
                }
            }

            ComboBox {
                id: colorSelector
                width: 180
                model: ["Blue", "Red", "Green", "Orange", "Purple"]
                currentIndex: 0
                onCurrentTextChanged: {
                    switch(currentText){
                        case "Blue": lineColor="blue"; break;
                        case "Red": lineColor="red"; break;
                        case "Green": lineColor="green"; break;
                        case "Orange": lineColor="orange"; break;
                        case "Purple": lineColor="purple"; break;
                    }
                    chartCanvas.requestPaint()
                }
            }
        }

        Canvas {
            id: chartCanvas
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: comboRow.bottom
            anchors.bottom: parent.bottom
            anchors.margins: 10

            onPaint: {
                var ctx = getContext("2d");
                ctx.reset();

                var left = 100, top = 50, right = width-50, bottom = height-100;

                ctx.fillStyle = "#ffffff";
                ctx.fillRect(0, 0, width, height);

                ctx.strokeStyle = "#000";
                ctx.lineWidth = 2;
                ctx.beginPath();
                ctx.moveTo(left, top);
                ctx.lineTo(left, bottom);
                ctx.lineTo(right, bottom);
                ctx.stroke();

                var minSQ = Math.min.apply(null, standardData.map(d => d.sq));
                var maxSQ = Math.max.apply(null, standardData.map(d => d.sq));
                var minCt = Math.min.apply(null, standardData.map(d => d.ct));
                var maxCt = Math.max.apply(null, standardData.map(d => d.ct));

                ctx.font = "16px Sans";
                ctx.textAlign = "center";
                ctx.fillStyle = "#000";

                var xTicks = [];
                if (xScaleType==="linear") {
                    var minPow = Math.floor(Math.log10(minSQ));
                    var maxPow = Math.ceil(Math.log10(maxSQ));
                    for (var p = minPow; p <= maxPow; p++) xTicks.push(Math.pow(10,p));
                } else {
                    var step = (maxSQ - minSQ)/5;
                    for (var i=minSQ; i<=maxSQ; i+=step) xTicks.push(i);
                }

                for(var i=0;i<xTicks.length;i++){
                    var xVal = xTicks[i];
                    var px = (xScaleType==="linear") ?
                        left + (Math.log10(xVal)-Math.log10(minSQ))/(Math.log10(maxSQ)-Math.log10(minSQ))*(right-left) :
                        left + (xVal-minSQ)/(maxSQ-minSQ)*(right-left);

                    ctx.beginPath();
                    ctx.moveTo(px, bottom);
                    ctx.lineTo(px, bottom+8);
                    ctx.stroke();

                    var label = (xScaleType==="linear") ? "10^"+Math.round(Math.log10(xVal)) : xVal.toFixed(1);
                    ctx.fillText(label, px, bottom+25);
                }

                ctx.textAlign = "right";
                var yStep = Math.ceil((maxCt-minCt)/5);
                for(var y=minCt; y<=maxCt; y+=yStep){
                    var py = bottom - (y-minCt)/(maxCt-minCt)*(bottom-top);
                    ctx.beginPath();
                    ctx.moveTo(left, py);
                    ctx.lineTo(left-8, py);
                    ctx.stroke();
                    ctx.fillText(y.toFixed(0), left-12, py+5);
                }

                ctx.textAlign = "center";
                ctx.font = "18px Sans";
                ctx.fillText("Starting Quantity", (left+right)/2, height-25);
                ctx.save();
                ctx.translate(40, (top+bottom)/2);
                ctx.rotate(-Math.PI/2);
                ctx.fillText("Ct (Cycle Threshold)", 0, 0);
                ctx.restore();

                ctx.strokeStyle = lineColor;
                ctx.lineWidth = 2;
                ctx.beginPath();
                for(var i=0;i<standardData.length;i++){
                    var sq = standardData[i].sq;
                    var ct = standardData[i].ct;
                    var px = (xScaleType==="linear") ?
                        left + (Math.log10(sq)-Math.log10(minSQ))/(Math.log10(maxSQ)-Math.log10(minSQ))*(right-left) :
                        left + (sq-minSQ)/(maxSQ-minSQ)*(right-left);
                    var py = bottom - (ct-minCt)/(maxCt-minCt)*(bottom-top);
                    if(i===0) ctx.moveTo(px, py); else ctx.lineTo(px, py);
                }
                ctx.stroke();

                ctx.fillStyle = lineColor;
                for(var i=0;i<standardData.length;i++){
                    var sq = standardData[i].sq;
                    var ct = standardData[i].ct;
                    var px = (xScaleType==="linear") ?
                        left + (Math.log10(sq)-Math.log10(minSQ))/(Math.log10(maxSQ)-Math.log10(minSQ))*(right-left) :
                        left + (sq-minSQ)/(maxSQ-minSQ)*(right-left);
                    var py = bottom - (ct-minCt)/(maxCt-minCt)*(bottom-top);
                    ctx.beginPath();
                    ctx.arc(px, py, 5, 0, 2*Math.PI);
                    ctx.fill();
                }
            }

            Component.onCompleted: requestPaint()
        }

        Button {
            id: saveButton
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 12
            width: 220
            height: 60
            property bool saved: false

            onClicked: {
                saved = true
                console.log("Standard curve plot saved!")
            }

            background: Rectangle {
                radius: 15
                color: saveButton.saved ? "#43A047" : "#1565C0"
                border.color: "#0D47A1"
                border.width: 2
            }

            contentItem: Row {
                spacing: 10
                anchors.centerIn: parent

                Text {
                    text: "\u{1F4BE}"
                    font.pixelSize: 28
                    color: "white"
                }

                Text {
                    text: saveButton.saved ? "Plot Saved!" : "Save Plot"
                    font.pixelSize: 24
                    font.family: "Verdana"
                    font.bold: true
                    color: saveButton.saved ? "#E8F5E9" : "#E3F2FD"
                }
            }
        }

    }
}
