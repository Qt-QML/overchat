import QtQuick 2.9
import QtQuick.Layouts 1.3

import "qrc:/components/singletons/."

Item {
    id: root

    anchors.fill: parent

    property string roomId: "";
    property alias messageListModel: messageList.model

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
            text: "Select room to start messaging"
        }
    }

    ColumnLayout {
        id: messageListWrapper

        anchors.fill: parent

        anchors.leftMargin: 4
        anchors.rightMargin: 8

        ListView {
            id: messageList

            Layout.fillWidth: true
            Layout.fillHeight: true

            verticalLayoutDirection: ListView.BottomToTop

            delegate: messageListDelegate
        }

        Rectangle {
            id: messageBox

            Layout.fillWidth: true
            Layout.preferredHeight: 50
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

                    text: modelData.text

                    wrapMode: "Wrap"
                }
            }
        }
    }

    states: [
        State {
            name: "empty"
            when: root.roomId == ""

            PropertyChanges {target: filler; opacity: 1; visible: true;}
            PropertyChanges {target: messageListWrapper; opacity: 0; visible: false;}
        },
        State {
            name: "filled"
            when: root.roomId != ""

            PropertyChanges {target: filler; opacity: 0; visible: false;}
            PropertyChanges {target: messageListWrapper; opacity: 1; visible: true;}
        }
    ]
}
