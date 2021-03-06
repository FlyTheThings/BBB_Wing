import QtQuick 2.6
import "../../Controls"

Rectangle {
    color: palette.middlegroundColor

    Column {
        id: column
        anchors.left: parent.left
        anchors.top: parent.top
        width: configService.value("Gui/Dashboard/width")

        Repeater {
            id: repeater
            model: [ { id: "gui", name: qsTr("Gui"), source: "GuiSettings.qml" }
            ]

            Button {
                text: modelData.name
                onClicked: loader.source = modelData.source
                width: parent.width
            }
        }
    }

    Loader {
        id: loader
        anchors.left: column.right
        anchors.right: parent.right
        anchors.top:  parent.top
        anchors.bottom: parent.bottom
        source: repeater.model[0].source
    }
}
