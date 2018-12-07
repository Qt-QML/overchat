import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "../elements" as Elements

Item {
    id: root

    readonly property int responsiveWidth: 500
    property int windowWidth: -1;
    property int windowHeight: -1;
    property bool wrapOpened: true;

    signal closed(string data);

    width: 300
    height: 400

    anchors.centerIn: parent

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Item {
            id: header

            Layout.alignment: Qt.AlignTop

            Layout.fillWidth: true
            height: 50

            RowLayout {
                anchors.fill: parent
                spacing: 0

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    color: "red"

                    Text {
                        leftPadding: 20
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 14
                        text: "Profile"
                    }
                }

                Button {
                    Layout.alignment: Qt.AlignRight

                    Layout.preferredWidth: 50
                    Layout.fillHeight: true

                    text: "X"
                    onClicked: root.close()
                }
            }
        }

        Item {
            id: body

            Layout.fillWidth: true
            Layout.fillHeight: true

            Rectangle {
                anchors.fill: parent

                color: "blue"
            }

            Item {
                id: auth

                anchors.fill: parent
                anchors.margins: 20

                Text {
                    id: authLabel

                    font.pixelSize: 14
                    text: "Authentication"
                }

                Button {
                    id: loginButton

                    anchors.top: authLabel.bottom
                    anchors.left: parent.left
                    anchors.topMargin: 5

                    text: "LOG IN"
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
