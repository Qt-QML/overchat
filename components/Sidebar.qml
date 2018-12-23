import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "./elements" as Elements
import "./models" as Models

import "qrc:/components/singletons/."

Item {
    id: root

    signal menuButtonClicked

    anchors.fill: parent


    Rectangle {
        anchors.fill: parent

        color: "lightblue"
    }

    ColumnLayout {
        anchors.fill: parent

        anchors.topMargin: 8
        spacing: 8

        RowLayout {
            id: header

            spacing: 8

            Layout.leftMargin: 8
            Layout.rightMargin: 8

            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true

            Elements.BurgerButton {
                id: menuButton

                Layout.alignment: Qt.AlignLeft
                Layout.preferredWidth: 50

                onClicked: menuButtonClicked()
            }

            Elements.TextField {
                id: searchField

                placeholderText: "Search..."

                Layout.fillWidth: true
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ListView {
                id: roomList

                anchors.fill: parent

                clip: true

                model: User.roomList

                delegate: roomListDelegate
            }
        }
    }

    Component {
        id: roomListDelegate

        Item {
            width: parent.width
            height: 50

            Component.onCompleted: function() {
                console.log("MDAT", modelData);
                room.setRoomId(modelData.id);
            }

            Rectangle {
                id: roomListItemBackground

                anchors.fill: parent

                color: roomListItemMouseArea.containsMouse ? "orange" : "transparent"
            }

            Item {
                anchors.fill: parent

                anchors.topMargin: 8
                anchors.leftMargin: 8

                Text {
                    text: modelData.name
                }
            }

            Models.Room {
                id: room
            }

            MouseArea {
                id: roomListItemMouseArea

                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.OpenHandCursor

                onClicked: function() {
                    chat.roomId = room.roomId;
                    chat.messageListModel = room.messageList;
                }
            }
        }
    }
}
