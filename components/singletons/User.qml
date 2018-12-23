pragma Singleton

import "../db/user.js" as UserDB
import "../essentials/functions.js" as Functions

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

    FirebaseUserBackend {
        id: backend

        onSigninCompleted: function(type, data) {
            if (type === "succ") {
                var params = backend.getAuthParams();
                UserDB.storeData(db, params);
            }

            backend.updateUserList();

            backend.subscribeRoomList();

            root.signinCompleted();
        }

        onUserListChanged: function() {
//            var ul = backend.userList;
        }

        onRoomListChanged: function() {
            console.log(backend.roomList);
        }
    }

    function loginLocal(cb) {
        UserDB.readData(db, function(auth_params) {
            if (auth_params) {
                Functions.connectOnce(backend.onSigninCompleted, cb);
                backend.signinByAuthParams(auth_params);
            } else {
                cb("void");
            }
        });
    }

    function login(email, password, name, isReg, cb) {
        var errors = [];
        Functions.validate("required", "email", email, errors);
        Functions.validate("required", "password", password, errors);

        if (isReg) {
            Functions.validate("required", "name", name, errors);
        }

        if (errors.length > 0) {
            cb("vald", errors);
            return;
        }

        Functions.connectOnce(backend.onSigninCompleted, cb);

        if (isReg) {
            backend.signup(email, password, name);
        } else {
            backend.signin(email, password);
        }
    }

    function logout() {
        backend.signout();
        UserDB.deleteData(db);
    }

    function getUserList() {
        // backend.getUsers();
        // remove self from list
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

//        Functions.connectOnce(backend.onCreateRoomCompleted, cb);
    }

    function getAuthParams() {
        return backend.getAuthParams();
    }
}
