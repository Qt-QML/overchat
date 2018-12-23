import QtQuick 2.9
import QtQuick.Layouts 1.3

import "qrc:/components/singletons/."

import "./elements" as Elements
import "./models" as Models

Item {
    id: root

    anchors.fill: parent

    property Models.Room room
    property string name: "<unnamed>"

    signal backButtonClicked()

    Rectangle {
        id: background

        anchors.fill: parent

        color: "#C6D8FF"

        Item {
            id: filler

            width: 260
            height: 40

            anchors.centerIn: parent

            Rectangle {
                anchors.fill: parent
                color: "#aaaaff"
                radius: 5
            }

            Text {
                anchors.centerIn: parent
                text: (room && room.messageCount === 0) ? "There are no messages yet" : "Select room to start messaging"
            }
        }

        ColumnLayout {
            id: messageListWrapper

            anchors.fill: parent

            Item {
                id: messageListHeaderWrapper

                Layout.fillWidth: true
                Layout.preferredHeight: 50

                Rectangle {
                    anchors.fill: parent

                    color: "#253443"
                }

                RowLayout {
                    anchors.fill: parent

                    Text {
                        Layout.margins: 8

                        font.pixelSize: 18
                        font.bold: true
                        color: "white"
                        text: root.name
                    }

                    Elements.SubmitButton {
                        id: backButton

                        Layout.margins: 8
                        Layout.preferredWidth: 70
                        Layout.alignment: Qt.AlignRight

                        text: "Back"

                        onClicked: function() {
                            root.backButtonClicked()
                        }
                    }
                }
            }

            ListView {
                id: messageList

                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: 8

                clip: true
                verticalLayoutDirection: ListView.BottomToTop

                delegate: messageListDelegate

                model: room ? room.messageList : 0
            }

            Item {
                id: messageListFooterWrapper

                Layout.fillWidth: true
                Layout.preferredHeight: 42

                Rectangle {
                    anchors.fill: parent

                    color: "#253443"
                }

                RowLayout {
                    anchors.fill: parent

                    Elements.TextField {
                        id: messageBox

                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.margins: 8
                        Layout.rightMargin: 4

                        placeholderText: "Write your message here..."

                        font.pixelSize: 14

                        background.implicitHeight: 20
                    }

                    Elements.SubmitButton {
                        id: sendButton

                        Layout.margins: 8
                        Layout.leftMargin: 4
                        Layout.rightMargin: 4
                        Layout.preferredWidth: 70

                        text: "Send"

                        onClicked: function() {
                            room.sendMessage(messageBox.text);
                            messageBox.clear();
                        }
                    }

                    Elements.SubmitButton {
                        id: attachButton

                        Layout.margins: 8
                        Layout.leftMargin: 4
                        Layout.preferredWidth: 30

                        text: "I"

                        onClicked: function() {

                        }
                    }
                }
            }
        }

        Component {
            id: messageListDelegate

            Item {
                width: parent.width
                height: 50

                Item {
                    width: messageText.width < 200 ? messageText.width : 200
                    height: parent.height

                    anchors.right: modelData.isAuthor ? parent.right : undefined

                    Text {
                        id: messageText

                        text: User.getUserNameById(modelData.authorId) + ': ' + modelData.text

                        wrapMode: "Wrap"
                    }
                }
            }
        }
    }

    states: [
        State {
            name: "empty"
            when: (!room || room.roomId === "") || (room && room.messageCount === 0)

            PropertyChanges {target: filler; opacity: 1; visible: true;}
            PropertyChanges {target: messageListWrapper; opacity: (room && room.messageCount === 0) ? 1 : 0; visible: (room && room.messageCount === 0) ? true : false;}
        },
        State {
            name: "filled"
            when: room && room.roomId !== ""

            PropertyChanges {target: filler; opacity: 0; visible: false;}
            PropertyChanges {target: messageListWrapper; opacity: 1; visible: true;}
        }
    ]
}
