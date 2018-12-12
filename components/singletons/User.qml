pragma Singleton
import QtQuick 2.9

QtObject {
    id: root

    property bool   authenticated: false

    property string accessToken: ""
    property string refreshToken: ""
    property int    expiresIn: -1

    property string id: ""
    property string email: ""

    function fillLogin(data) {
        root.accessToken    = data.idToken;
        root.refreshToken   = data.refreshToken;

        root.id     = data.localId;
        root.email  = data.email;

        root.authenticated = true;
    }

    function cleanup() {
        root.authenticated = false;

        root.accessToken = "";
        root.refreshToken = "";
        root.expiresIn = "";

        root.id = "";
        root.email = "";
    }
}
