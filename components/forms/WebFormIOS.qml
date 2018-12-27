import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import IOSWebView 1.0

import "qrc:/components/singletons/."

import "../elements" as Elements
import "./fieldsets" as Fieldsets

import '../essentials/URLQuery.js' as URLQuery

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
    height: 400

    anchors.centerIn: parent

    Component.onCompleted: function() {
        login();
    }

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
                        text: "Authentication"
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

        IOSWebView {
            id: webView

            Layout.fillWidth: true
            Layout.fillHeight: true

            onUrlChanged: function(data) {
//                console.log("URLOK", data);
            }

            onPageFinished: function(url) {
//                console.log("PAGE URL:", url);

                var result = URLQuery.parseParams(url);

                if (result.code) {
                    User.loginOauth(result.code, function() {
                        root.close();
                    });
                }
            }

            onPageError: function(code, error, url) {
//                console.log("URLFAIL", url);

                var result = URLQuery.parseParams(url);

                if (result.code) {
//                    console.log("code", result.code);

                    User.loginOauth(result.code, function() {
                        root.close();
                    });
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

    function login() {
        var params = {
            response_type: 'code',
            scope: "https://www.googleapis.com/auth/userinfo.email https://www.googleapis.com/auth/firebase.database https://www.googleapis.com/auth/cloud-platform",
            redirect_uri: 'https://overchat-e401f.firebaseapp.com/__/auth/handler',
            client_id: "35451728355-c0uv4n0534buliuobqti2u8522261bo6.apps.googleusercontent.com",
            access_type: 'offline'
        }

        var url = "https://accounts.google.com/o/oauth2/auth?%1".arg(URLQuery.serializeParams(params));

        webView.setUrl(url);
    }

    function close() {
        root.closed("data: ww" + windowWidth);
    }
}
