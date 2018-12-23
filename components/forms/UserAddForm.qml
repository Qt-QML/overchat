import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/components/singletons/."

import "../elements" as Elements
import "./fieldsets" as Fieldsets

Item {
    id: root

    readonly property int responsiveWidth: 500
    property int windowWidth: -1;
    property int windowHeight: -1;
    property bool wrapOpened: true;

    property string userId: "";
    property string userName: "Select User";

    signal closed(string data);

    width: 300
    height: 200

    anchors.centerIn: parent

    Rectangle {
        anchors.fill: parent
        color: "#071019"
        radius: 4
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Item {
            id: header

            Layout.alignment: Qt.AlignTop

            Layout.fillWidth: true
            height: 50

            RowLayout {
                anchors.margins: 10
                anchors.leftMargin: 20
                anchors.fill: parent
                spacing: 20

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 18
                        font.bold: true
                        color: "white"
                        text: "Add User"
                    }
                }

                Elements.CloseButton {
                    Layout.alignment: Qt.AlignRight

                    Layout.preferredWidth: 30
                    Layout.fillHeight: true

                    onClicked: root.close()
                }
            }
        }

        Item {
            id: body

            Layout.fillWidth: true
            Layout.fillHeight: true

            Layout.margins: 20

            ComboBox {
                id: userComboBox

                width: 200
                model: User.userList

                delegate: ItemDelegate {
                    text: modelData.name

                    onClicked: function() {
                        root.userId = modelData.id;
                        root.userName = modelData.name;
                    }
                }

                displayText: root.userName
            }
        }

        Elements.SubmitButton {
            id: logoutButton

            Layout.margins: 20

            text: "ADD USER"

            onClicked: function() {
                if (root.userId && root.userName) {
                    User.createRoom(root.userId, root.userName);
                }
            }
        }

        states: [
            State {
                when: windowWidth >= responsiveWidth

                PropertyChanges { target: root; width: responsiveWidth - 50 }
            },
            State {
                when: windowWidth < responsiveWidth

                PropertyChanges { target: root; width: windowWidth }
            }
        ]
    }

    function close() {
        root.closed("data: ww" + windowWidth);
    }
}
