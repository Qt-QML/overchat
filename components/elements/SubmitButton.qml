import QtQuick 2.9
import QtQuick.Controls 2.2

Button {
    id: root

    Component.onCompleted: function() {
        background.implicitHeight = 20
        background.radius = 4
        background.color = "#152433"
        background.border.color = "transparent"
        contentItem.color = "white"
    }
}
