import QtQuick 2.9

Item {
    id: root

    property int windowWidth: -1;
    property int windowHeight: -1;
    property bool opened: false;

    anchors.fill: parent

    Rectangle {
        id: dimmer
        anchors.fill: parent

        color: "black"

        MouseArea {anchors.fill: parent; onClicked: root.opened = false;}
    }

    Item {
        id: loaderWrapper
        anchors.fill: parent

        Loader {
            id: formLoader
            anchors.centerIn: parent

            onLoaded: {
                binderW.target = formLoader.item;
                binderH.target = formLoader.item
            }

            MouseArea {anchors.fill: parent;}
        }

        Binding {id: binderW; property: "windowWidth"; value: root.windowWidth;}
        Binding {id: binderH; property: "windowHeight"; value: root.windowHeight;}

        Connections {
            target: formLoader.item
            onClosed: function(data) {
                console.log(data);
                root.close();
            }
        }
    }

    states: [
        State {
            name: "closed"
            when: root.opened === false

            PropertyChanges {target: dimmer; opacity: 0; visible: false;}
            PropertyChanges {target: loaderWrapper; opacity: 0; visible: false;}
        },
        State {
            name: "opened"
            when: root.opened === true

            PropertyChanges {target: dimmer; opacity: 0.4; visible: true;}
            PropertyChanges {target: loaderWrapper; opacity: 1; visible: true;}
        }
    ]

    transitions: [
        Transition {
            from: "closed"; to: "opened"
            PropertyAnimation { target: dimmer; properties: "opacity"; duration: 300; easing.type: Easing.OutQuad }
        },
        Transition {
            from: "opened"; to: "closed"
            PropertyAnimation { target: dimmer; properties: "opacity, visible"; duration: 300; easing.type: Easing.InQuad }
            PropertyAnimation { target: loaderWrapper; properties: "opacity"; duration: 300; easing.type: Easing.OutQuad }
        }
    ]

    function open(form) {
        formLoader.source = form;
        root.opened = true;
    }

    function close() {
        root.opened = false;
    }
}
