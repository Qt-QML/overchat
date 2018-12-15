pragma Singleton

import "../db/user.js" as UserDB
import "../essentials/functions.js" as Functions

import QtQuick 2.9
import QtQuick.LocalStorage 2.0
import Firebase 1.0

Item {
    id: root

    readonly property bool   authenticated: backend.email
    readonly property string email: backend.email
    readonly property string name: backend.name

    property var db;

    Component.onCompleted: {
        db = LocalStorage.openDatabaseSync("overchat", "1.0", "An overchat's database", 100000);

        UserDB.initDatabase(db);
    }

    FirebaseBackend {
        id: backend

        onResult: function(type, data) {
            if (type === "succ") {
                var params = backend.getAuthParams();
                UserDB.storeData(db, params);
            }
        }
    }

    function loginLocal(cb) {
        UserDB.readData(db, function(auth_params) {
            if (auth_params) {
                Functions.connectOnce(backend.onResult, cb);
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

        Functions.connectOnce(backend.onResult, cb);

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
}
