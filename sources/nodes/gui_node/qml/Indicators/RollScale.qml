import QtQuick 2.5
import "qrc:/js/helpers/drawer_helper.js" as Helper

Item {
    id: rollScale

    property int roll: 0
    property int minRoll
    property int maxRoll
    property int valueStep: 5

    property int offset

    width: height

    onRollChanged: canvas.requestPaint()

    Canvas {
        id: canvas
        anchors.fill: parent
        onPaint: {
            var ctx = canvas.getContext('2d');

            ctx.clearRect(0, 0, width, height);

            ctx.lineWidth = 2;
            ctx.strokeStyle = palette.textColor;
            ctx.fillStyle = palette.textColor;
            ctx.font = '11pt Open Sans';
            ctx.textBaseline = 'middle';
            ctx.textAlign = 'center';

            ctx.save();
            ctx.beginPath();

            ctx.translate(width / 2, height / 2);
            for (var i = minRoll - (minRoll % valueStep); i <= maxRoll;
                 i += valueStep) {
                ctx.save();
                ctx.rotate(i * Math.PI / 180);
                ctx.moveTo(0, (i % 15 ? offset / 2 : 0) - height / 2);
                ctx.lineTo(0, offset - height / 2);
                ctx.restore();
            }

            ctx.save();
            ctx.rotate(-roll * Math.PI / 180);
            ctx.moveTo(0, offset - height / 2);
            ctx.lineTo(16, offset + 16 - height / 2);
            ctx.lineTo(-16, offset + 16 - height / 2);
            ctx.lineTo(0, offset - height / 2);

            ctx.font = '14pt Open Sans';
            ctx.fillText(roll, 0, offset + 32 - height / 2);

            ctx.restore();

            ctx.stroke();
            ctx.restore();
        }
    }
}
