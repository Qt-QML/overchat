import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "./elements" as Elements

import "qrc:/components/singletons/."

Item {
    id: root

    signal menuButtonClicked

    anchors.fill: parent

    ColumnLayout {
        anchors.fill: parent

        anchors.topMargin: 8
        anchors.leftMargin: 8
        anchors.rightMargin: 8
        spacing: 8

        RowLayout {
            id: header

            spacing: 8

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

            Rectangle {
                anchors.fill: parent

                color: "lightblue"
            }

            ListView {
                id: roomList

                anchors.fill: parent

                clip: true

                model: User.roomList

                delegate: roomListDelegate
                spacing: 5
            }

            Component {
                id: roomListDelegate

                Text {
                    text: modelData.name
                }
            }
        }




    }
}
