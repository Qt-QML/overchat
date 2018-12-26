pragma Singleton

import "../db/user.js" as UserDB
import "../essentials/functions.js" as Functions
import "../essentials/requests.js" as Requests

import QtQuick 2.9
import QtQuick.LocalStorage 2.0
import FirebaseUser 1.0

Item {
    id: root

    signal signinCompleted

    readonly property bool   authenticated: backend.email
    readonly property string email: backend.email
    readonly property string name: backend.name
    readonly property alias  userList: backend.userList
    readonly property alias  roomList: backend.roomList

    property var db;

    Component.onCompleted: {
        db = LocalStorage.openDatabaseSync("overchat", "1.0", "An overchat's database", 100000);

        UserDB.initDatabase(db);
    }

    QtObject {
        id: priv
        property var userDB: ({})
    }

    FirebaseUserBackend {
        id: backend

        onSigninCompleted: function(type, data) {
            if (type === "succ") {
                var params = backend.getAuthParams();
                UserDB.storeData(db, params);

                console.log("DB SAVE", JSON.stringify(params));
            }

            backend.updateUserList();

            backend.subscribeRoomList();

            root.signinCompleted();
        }

        onUserListChanged: function() {
            var ul = backend.userList;

            priv.userDB = {};

            for (var i = 0; i < ul.length; i++) {
                priv.userDB[ul[i].id] = ul[i].name;
            }
        }

        onRoomListChanged: function() {
            console.log(backend.roomList);
        }
    }

    function loginLocal(cb) {
        UserDB.readData(db, function(auth_params) {
            if (auth_params) {
                Functions.connectOnce(backend.onSigninCompleted, cb);
                var refresh_token = auth_params["refreshToken"];

                if (refresh_token) {
                    Requests.refreshToken(refresh_token, function(access_token, expires_in) {
                        backend.signinOauth(access_token, refresh_token, expires_in);
                        cb();
                    });
                } else {
                    cb("void");
                }
            } else {
                cb("void");
            }
        });
    }

    function loginOauth(code, cb) {
        UserDB.readData(db, function(auth_params) {
            var refresh_token_saved = "";

            console.log("DATAREAD", JSON.stringify(auth_params));

            if (auth_params && auth_params["refreshToken"]) {
                refresh_token_saved = auth_params["refreshToken"];
                console.log("SAVEDSAVED", refresh_token_saved);
            }

            Requests.getUserCredentials(code, function(auth_data, email, name) {
                cb();

                var access_token = auth_data["access_token"];
                var refresh_token = auth_data["refresh_token"];
                var expires_in = auth_data["expires_in"];

                console.log("REQ DAT", JSON.stringify(auth_data));

                refresh_token = refresh_token ? refresh_token : refresh_token_saved;

                if (refresh_token === "") {
//                    console.log("ERROR: NO REFRESH TOKEN EXIST!");
//                    return;
                }

                if (auth_data["registered"]) {
                    console.log("LOGIN OAUTH");
                    backend.signinOauth(access_token, refresh_token, expires_in);
                } else {
                    console.log("SIGNUP OAUTH", auth_data["access_token"]);
                    backend.signupOauth(access_token, refresh_token, expires_in);
                }
            });
        });
    }

    function logout() {
        backend.signout();
        UserDB.deleteData(db);
    }

    function createRoom(user_id, room_name, cb) {
        var errors = [];
        Functions.validate("required", "user_id", user_id, errors);
        Functions.validate("required", "room_name", room_name, errors);

        backend.createRoom(user_id, room_name);

        if (errors.length > 0) {
            cb("vald", errors);
            return;
        }
    }

    function getAuthParams() {
        return backend.getAuthParams();
    }

    function getUserNameById(user_id) {
        var user_name = priv.userDB[user_id];

        return user_name ? user_name : "<unknown>"
    }
}
