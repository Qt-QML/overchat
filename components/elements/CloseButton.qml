import QtQuick 2.9
import QtQuick.Controls 2.2

Button {
    id: root

    text: "X"

    Component.onCompleted: function() {
        background.color = "#071019"
        background.border.color = "transparent"
        contentItem.color = "white"
    }
}
