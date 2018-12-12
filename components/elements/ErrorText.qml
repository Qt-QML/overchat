import QtQuick 2.9
import QtQuick.Controls 2.2

Text {
    id: root

    color: "red"

    function showErrors(errors) {
        root.text = JSON.stringify(errors);
    }

    function clear() {
        root.text = "";
    }
}
