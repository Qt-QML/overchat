import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/components/singletons/."

import "../../elements" as Elements

import "../../requests/authentication.js" as AuthenticationRequests

Item {
    id: root

    signal login(string test)
    signal logout(string test)
    signal register(string test)

    anchors.fill: parent
    anchors.margins: 20

    ColumnLayout {
        id: layout
        width: parent.width

        Text {
            id: authLabel

            Layout.alignment: Qt.AlignTop

            font.pixelSize: 12
            text: "Account"
            color: "white"
        }

        ColumnLayout {
            id: labels

            Text {
                id: fullnameLabel

                font.pixelSize: 20
                font.bold: true

                color: "white"
                text: "John Doe"
            }

            Text {
                id: emailLabel

                font.pixelSize: 14

                color: "white"
                text: User.email ? User.email : "<noemail>"
            }
        }

        GridLayout {
            id: fields

            width: parent.width
            height: 20

            columns: 2

            Elements.TextField {
                id: emailField

                Layout.fillWidth: true
                Layout.fillHeight: true

                placeholderText: "E-mail"

                //TODO: Remove
                text: "sanekmolodoy@gmail.com"

                font.pixelSize: 14

                background.implicitHeight: 20
            }

            Elements.TextField {
                id: passwordField

                Layout.fillWidth: true
                Layout.fillHeight: true

                placeholderText: "Password"

                echoMode: TextInput.Password

                font.pixelSize: 14

                background.implicitHeight: 20
            }
        }

        RowLayout {
            id: buttons

            Elements.SubmitButton {
                id: loginButton

                text: "LOG IN"

                onClicked: function() {
                    errorText.clear();

                    AuthenticationRequests.login(emailField.text, passwordField.text, onresponse);
                }

                function onresponse(res) {
                    if (res.type === "vald") {errorText.showErrors(res.data)}
                    if (res.type === "succ") {processLogin(res.data)}
                    if (res.type === "fail") {console.log("ERR!", res.data.error.message, res.data)}
                }
            }

            Elements.SubmitButton {
                id: registerButton

                text: "REGISTER"

                onClicked: function() {
                    errorText.clear();

                    AuthenticationRequests.register(emailField.text, passwordField.text, onresponse);
                }

                function onresponse(res) {
                    if (res.type === "vald") {errorText.showErrors(res.data)}
                    if (res.type === "succ") {processRegister(res.data)}
                    if (res.type === "fail") {console.log("ERR!", res.data.error.message, res.data)}
                }
            }

            Elements.SubmitButton {
                id: logoutButton

                text: "LOG OUT"

                onClicked: function() {
                    processLogout();
                }
            }
        }

        Elements.ErrorText {
            id: errorText
        }
    }

    states: [
        State {
            when: User.authenticated === false
            PropertyChanges { target: labels; visible: false }
            PropertyChanges { target: fields; visible: true}
            PropertyChanges { target: registerButton; visible: true}
            PropertyChanges { target: loginButton; visible: true}
            PropertyChanges { target: logoutButton; visible: false}
        },
        State {
            when: User.authenticated === true
            PropertyChanges { target: labels; visible: true }
            PropertyChanges { target: fields; visible: false }
            PropertyChanges { target: registerButton; visible: false}
            PropertyChanges { target: loginButton; visible: false}
            PropertyChanges { target: logoutButton; visible: true}
        }
    ]

    function processLogin(data) {
        User.fillLogin(data);

        root.login("test");
    }

    function processRegister(data) {
        User.fillLogin(data);

        root.register("test");
    }

    function processLogout(data) {
        User.cleanup();

        root.logout("test");
    }
}
