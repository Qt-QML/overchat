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
                console.log("SIGNIN COMPLETED, SAVING AUTH DATA: ", JSON.stringify(params))

                UserDB.storeData(db, params);
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
//            console.log(backend.roomList);
        }
    }

    function loginLocal(cb) {
        console.log("User - loginLocal()")

        UserDB.readData(db, function(auth_params) {
            console.log("...data read", JSON.stringify(auth_params));

            if (auth_params) {
                Functions.connectOnce(backend.onSigninCompleted, cb);
                var refresh_token = auth_params["refreshToken"];

                if (refresh_token) {
                    console.log("...refresh token exists");
                    Requests.refreshToken(refresh_token, function(access_token, expires_in) {
                        console.log("...access token refreshed, signing in");
                        backend.signinOauth(access_token, refresh_token, expires_in);
                        cb();
                    });
                } else {
                    console.log("...refresh token does not exist, skipping");
                    cb("void");
                }
            } else {
                console.log("...auth params does not exist, skipping");

                cb("void");
            }
        });
    }

    function loginOauth(code, cb) {
        console.log("User - loginOauth()");

        UserDB.readData(db, function(auth_params) {
            console.log("...data read", JSON.stringify(auth_params));
            var refresh_token_saved = "";

            if (auth_params && auth_params["refreshToken"]) {
                console.log("...refresh token exists");

                refresh_token_saved = auth_params["refreshToken"];
            } else {
                console.log("...refresh token does not exist");
            }

            console.log("...getting credentials");
            Requests.getUserCredentials(code, function(auth_data) {
                console.log("...got credentials", JSON.stringify(auth_data));
                cb();

                var access_token = auth_data["access_token"];
                var refresh_token = auth_data["refresh_token"];
                var expires_in = auth_data["expires_in"];

                refresh_token = refresh_token ? refresh_token : refresh_token_saved;

                console.log("...refresh token is empty in doth DB and credentials");

                if (refresh_token === "") {
                    console.log("...refresh token is empty in doth DB and credentials");
                    // TODO: Need to revoke by access token
                }

//                if (auth_data["registered"]) {
//                    console.log("...user is registered, signin now");
//                    backend.signinOauth(access_token, refresh_token, expires_in);
//                } else {
//                    console.log("...user is not registered, signup now");
                    backend.signupOauth(access_token, refresh_token, expires_in);
//                }
            });
        });
    }

    function logout() {
        console.log("User - logout()");
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
