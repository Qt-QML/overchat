import QtQuick 2.9
import QtQuick.Controls 2.0

TextField {
    id: root

    color: "white"

    Component.onCompleted: function() {
        background.radius = 4
        background.color = "#152433"
        background.border.color = "transparent"
    }
}
