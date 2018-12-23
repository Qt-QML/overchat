import QtQuick 2.9
import QtQuick.Controls 2.4

TextField {
    id: root

    color: "white"

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.IBeamCursor
    }

    Component.onCompleted: function() {
        background.radius = 4
        background.color = "#152433"
        background.border.color = "transparent"
    }
}
