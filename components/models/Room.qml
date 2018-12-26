import "../essentials/functions.js" as Functions

import "qrc:/components/singletons/."

import QtQuick 2.9
import QtQuick.LocalStorage 2.0
import FirebaseRoom 1.0

Item {
    id: root

    readonly property alias roomId: backend.roomId
    readonly property alias messageList: messageListModel

    property int messageCount: 0

    FirebaseRoomBackend {
        id: backend

        onMessageListChanged: function(author_id, text, is_author, attachment) {
            if (!(author_id && text && (is_author !== null))) return;

            messageListModel.insert(0, {"modelData":
                {
                    "authorId": author_id,
                    "text": text,
                    "isAuthor": is_author,
                    "attachment": attachment
                }
            });

            root.messageCount = root.messageCount + 1;
        }
    }

    function setRoomId(room_id) {
        if (room_id === "") {
            console.log("room id is empty");
            return;
        }

        backend.setRoomId(room_id);
        updateAuthParams();
        User.onSigninCompleted.connect(updateAuthParams);

        backend.subscribeMessageList();

        root.messageCount = 0;
    }

    function updateAuthParams() {
        var aps = User.getAuthParams();
        backend.setAuthParams(aps);
    }

    function sendMessage(text, filePath) {
        if (text === "") {
            console.log("message text is empty");
            return;
        }

        if (!filePath) {
            filePath = "";
        }

        backend.sendMessage(text, filePath);
    }

    ListModel {
        id: messageListModel
    }
}
