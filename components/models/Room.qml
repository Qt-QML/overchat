import "../essentials/functions.js" as Functions

import "qrc:/components/singletons/."

import QtQuick 2.9
import QtQuick.LocalStorage 2.0
import FirebaseRoom 1.0

Item {
    id: root

    readonly property alias roomId: backend.roomId
    readonly property alias messageList: backend.messageList

    FirebaseRoomBackend {
        id: backend
    }

    function setRoomId(room_id) {
        if (room_id === "") {
            console.log("room_id is empty");
            return;
        }

        backend.setRoomId(room_id);
        updateAuthParams();
        User.onSigninCompleted.connect(updateAuthParams);

        backend.subscribeMessageList();
    }

    function updateAuthParams() {
        var aps = User.getAuthParams();
        backend.setAuthParams(aps);
    }
}
