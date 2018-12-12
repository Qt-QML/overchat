import QtQuick 2.9
import QtQuick.Controls 2.2

Button {
    id: root

    text: "M"

    Component.onCompleted: function() {
        background.radius = 4
        background.color = "black"
        background.border.color = "transparent"
        contentItem.color = "white"
    }
}
