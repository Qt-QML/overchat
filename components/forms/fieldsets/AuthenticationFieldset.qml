import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "qrc:/components/singletons/."

import "../../elements" as Elements

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
                text: User.name ? User.name : "<name>"
            }

            Text {
                id: emailLabel

                font.pixelSize: 14

                color: "white"
                text: User.email ? User.email : "<noemail>"
            }
        }

        ColumnLayout {
            id: fields

            width: parent.width

            GridLayout {
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

            Elements.TextField {
                id: nameField

                Layout.fillWidth: true
                Layout.fillHeight: true

                placeholderText: "Name"

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

                    User.login(emailField.text, passwordField.text, nameField.text, false, onresult);
                }
            }

            Elements.SubmitButton {
                id: registerButton

                text: "REGISTER"

                onClicked: function() {
                    errorText.clear();

                    User.login(emailField.text, passwordField.text, nameField.text, true, onresult);
                }
            }

            Elements.SubmitButton {
                id: logoutButton

                text: "LOG OUT"

                onClicked: function() {
                    User.logout();
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
//            PropertyChanges { target: labels; visible: false }
//            PropertyChanges { target: fields; visible: true}
//            PropertyChanges { target: registerButton; visible: true}
//            PropertyChanges { target: loginButton; visible: true}
//            PropertyChanges { target: logoutButton; visible: false}
            PropertyChanges { target: labels; visible: true }
            PropertyChanges { target: fields; visible: false }
            PropertyChanges { target: registerButton; visible: false}
            PropertyChanges { target: loginButton; visible: false}
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

    function onresult(type, data) {
        if (type === "vald") {errorText.showErrors(data)}
        if (type === "succ") {processLogin(data)}
        if (type === "fail") {console.log("ERR!", data.error.message, data)}
    }

    function processLogin(data) {
        root.login("test");
    }

    function processRegister(data) {
        root.register("test");
    }

    function processLogout(data) {
        root.logout("test");
    }
}
