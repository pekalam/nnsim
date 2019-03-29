import QtQuick 2.9
import QtQuick.Controls 2.0

Item{
    width: 550
    height: 410
    property int classNumber: -1

    Connections{
        target: backend
        onNetworkResponseChanged: {
            console.log(backend.networkResponse)
            responseText.text = backend.networkResponse
        }
    }

    Canvas {
        id: mycanvas
        width: 400
        height: 400
        property int pix: 256
        property double pixSz: 0
        property bool isDrawn: false
        onPaint: {
            if(isDrawn)
                return;
            var totalSz = mycanvas.width * mycanvas.height;
            pixSz = Math.sqrt(totalSz / mycanvas.pix);
            var pixX = mycanvas.width / pixSz;
            var pixY = mycanvas.height / pixSz;
            console.log(pixX);
            console.log(pixY);
            var ctx = mycanvas.getContext('2d');
            ctx.clearRect(0, 0, mycanvas.width, mycanvas.height);
            ctx.fillStyle = Qt.rgba(0, 0, 0, 1);
            ctx.fillRect(0, 0, mycanvas.width, mycanvas.height);
            var start = pixSz;
            for (var i = 0; i < pixX; i++) {
                ctx.beginPath();
                ctx.strokeStyle = Qt.rgba(1, 1, 1, 1);
                ctx.lineWidth = 2;
                ctx.moveTo(start, 0);
                ctx.lineTo(start, mycanvas.height);
                ctx.stroke();
                start = start + pixSz;
            }
            start = pixSz;
            for (i = 0; i < pixY; i++) {
                ctx.beginPath();
                ctx.strokeStyle = Qt.rgba(1, 1, 1, 1);
                ctx.lineWidth = 2;
                ctx.moveTo(0, start);
                ctx.lineTo(mycanvas.width, start);
                ctx.stroke();
                start = start + pixSz;
            }
            isDrawn = true
        }

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
            property int lastX: 0
            property int lastY: 0
            onPressed: {
                lastX = mouseX;
                lastY = mouseY;
            }

            onPositionChanged: {
                var x = mouseX;
                var y = mouseY;
                if(Math.sqrt( Math.pow(Math.abs(x-lastX),2)+Math.pow(Math.abs(y-lastY),2) ) < 15)
                    return;
                var ctx = mycanvas.getContext('2d');

                var rctX = Math.floor(x - x % parent.pixSz);
                var rctY = Math.floor(y - y % parent.pixSz);

                ctx.lineWidth = 7;
                ctx.strokeStyle = "white";
                ctx.lineJoin = "round";
                ctx.lineCap = "round";
                ctx.beginPath();
                ctx.moveTo(lastX, lastY);
                ctx.lineTo(x, y);
                ctx.stroke();
                backend.setAt(Math.ceil(rctX / parent.pixSz), Math.ceil(rctY / parent.pixSz), 1);
                lastX = x; lastY = y;
                mycanvas.requestPaint();
            }
        }
    }

    Text{
        id: responseText
        x: 420
    }

    Button{
        x: 420
        y: 370
        height: 30
        text: "Predict"
        onClicked: {
            backend.predict();
        }
    }
    Button{
        x: 420
        y: 320
        height: 30
        text: "Clear"
        onClicked: {
            mycanvas.isDrawn = false;
            mycanvas.requestPaint();
            backend.clear();
        }
    }

}


