pragma Singleton
import QtQuick 2.9
import QtQuick.LocalStorage 2.0

QtObject {
    id: root

    property bool   authenticated: false

    property string accessToken: ""
    property string refreshToken: ""
    property int    expiresIn: -1

    property string uid: ""
    property string email: ""

    // ссылка на объект БД
    property var db;

    Component.onCompleted: {
        initDatabase();
        readData();
    }

    function initDatabase() {
        print('initDatabase()')

        db = LocalStorage.openDatabaseSync("overchat", "1.0", "An overchat's database", 100000);
        db.transaction( function(tx) {
            print('... create table')
//            tx.executeSql("DROP TABLE user");
            tx.executeSql('CREATE TABLE IF NOT EXISTS user(id INT, localId INT, email TEXT, idToken TEXT, refreshToken TEXT, expiresIn INT)');
        });
    }

    function storeData() {
        print('storeData()')
        if (!db) {
            return;
        }

        db.transaction( function(tx) {
            print('... check if a user object exists')
            var result = tx.executeSql('SELECT * from user WHERE id=1');
            // prepare object to be stored as JSON
            var obj = [root.uid, root.email, root.accessToken, root.refreshToken, root.expiresIn];

            if(result.rows.length === 1) {
                // use update
                print('... user exists, update it')
                result = tx.executeSql('UPDATE user SET localId=?, email=?, idToken=?, refreshToken=?, expiresIn=?  WHERE id=1', obj);
            } else {
                // use insert
                print('... user does not exists, create it')
                result = tx.executeSql('INSERT INTO user VALUES (1,?,?,?,?,?)', obj);
            }
        });
    }

    function readData() {
        print('readData()')
        if (!db) {
            cleanup();
            return;
        }

        db.transaction( function(tx) {
            print('... read user object')
            var result = tx.executeSql('SELECT * FROM user WHERE id=1');
            if (result.rows.length === 1) {
                print('... update User singleton')
                var obj = result.rows[0];

                fillLogin(obj);
            } else {
                cleanup();
            }
        });
    }

    function deleteData() {
        print('deleteData()')
        if (!db) {
            return;
        }

        db.transaction( function(tx) {
            print('... delete user object')
            tx.executeSql('DELETE FROM user WHERE id=1');
        });
    }

    function fillLogin(data) {
        root.accessToken    = data.idToken;
        root.refreshToken   = data.refreshToken;
        root.expiresIn      = data.expiresIn;

        root.uid    = data.localId;
        root.email  = data.email;

        storeData();

        root.authenticated = true;
    }

    function cleanup() {
        root.authenticated = false;

        deleteData();

        root.accessToken = "";
        root.refreshToken = "";
        root.expiresIn = "";

        root.uid = "";
        root.email = "";
    }
}
