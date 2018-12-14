import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/components/singletons/."

import "./elements" as Elements

Item {
    id: root

    property bool opened: false;
    property alias items: sidepaneItems.model;

    anchors.fill: parent

    state: "closed"

    Rectangle {
        id: dimmer

        anchors.fill: parent

        color: "black"

        MouseArea {anchors.fill: parent; onClicked: root.opened = false;}
    }

    Item {
        id: sidepane
        anchors.left: parent

        width: 200
        height: parent.height

        Rectangle {
            anchors.fill: parent

            color: "blue"
        }

        ColumnLayout {
            anchors.fill: parent

            Item {
                id: sidepaneHeader

                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true
                height: 80

                Rectangle {
                    id: sidepaneHeaderBG

                    anchors.fill: parent

                    color: "green"
                }

                Text {
                    id: sidepaneUserEmail

                    anchors.fill: parent

                    text: User.email

                    font.pixelSize: 12

                    elide: Text.ElideRight
                }
            }

            ListView {
                id: sidepaneItems

                Layout.fillWidth: true
                Layout.fillHeight: true

                model: 10
                delegate: sidepaneDelegate

                clip: true
            }
        }
    }

    Component {
        id: sidepaneDelegate

        Item {
            width: ListView.view.width
            height: 40

            Rectangle {
                id: sidepaneItemBackground

                anchors.fill: parent

                color: sidepaneItemMousearea.containsMouse ? "grey" : "cyan"
            }

            Text {
                text: label ? label : "Item #" + index
                color: "black"
            }

            MouseArea {
                id: sidepaneItemMousearea

                anchors.fill: parent

                hoverEnabled: true
                onClicked: function() {
                    root.opened = false;
                    onclick();
                }
            }
        }
    }

    states: [
        State {
            name: "closed"
            when: root.opened == false

            PropertyChanges {target: dimmer; opacity: 0; visible: false;}
            PropertyChanges {target: sidepane; x: -sidepane.width;}
        },
        State {
            name: "opened"
            when: root.opened == true

            PropertyChanges {target: dimmer; opacity: 0.4; visible: true;}
            PropertyChanges {target: sidepane; x: 0;}
        }
    ]

    transitions: [
        Transition {
            from: "closed"; to: "opened"
            PropertyAnimation { target: dimmer; properties: "opacity"; duration: 300; easing.type: Easing.OutQuad }
            PropertyAnimation { target: sidepane; properties: "x"; duration: 300; easing.type: Easing.OutQuad }
        },
        Transition {
            from: "opened"; to: "closed"
            PropertyAnimation { target: dimmer; properties: "opacity, visible"; duration: 300; easing.type: Easing.InQuad }
            PropertyAnimation { target: sidepane; properties: "x"; duration: 300; easing.type: Easing.InQuad }
        }
    ]
}
