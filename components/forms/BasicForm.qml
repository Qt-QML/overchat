import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "../elements" as Elements
import "./fieldsets" as Fieldsets

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
                        text: "Profile"
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

            Fieldsets.AuthenticationFieldset {
                id: auth


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
