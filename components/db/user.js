function initDatabase(db) {
    print('initDatabase()')

    db.transaction( function(tx) {
        print('... create table')
//        tx.executeSql("DROP TABLE user");
        tx.executeSql('CREATE TABLE IF NOT EXISTS user(id INT, localId INT, email TEXT, idToken TEXT, refreshToken TEXT, expiresIn INT, displayName TEXT)');
    });
}

function storeData(db, data) {
    print('storeData()')
    if (!db || Object.keys(data).length == 0) {
        return;
    }

    db.transaction( function(tx) {
        print('... check if a user object exists')
        var result = tx.executeSql('SELECT * from user WHERE id=1');
        // prepare object to be stored as JSON
        var obj = [data.localId, data.email, data.accessToken, data.refreshToken, data.expiresIn, data.displayName];

        if(result.rows.length === 1) {
            // use update
            print('... user exists, update it')
            result = tx.executeSql('UPDATE user SET localId=?, email=?, idToken=?, refreshToken=?, expiresIn=?, displayName=?  WHERE id=1', obj);
        } else {
            // use insert
            print('... user does not exists, create it')
            result = tx.executeSql('INSERT INTO user VALUES (1,?,?,?,?,?,?)', obj);
        }
    });
}

function readData(db, cb) {
    print('readData()')
    if (!db) {
        return;
    }

    db.transaction( function(tx) {
        print('... read user object')
        var result = tx.executeSql('SELECT * FROM user WHERE id=1');
        if (result.rows.length === 1) {
            print('... update User singleton')
            var obj = result.rows[0];

            cb(obj);
        } else {
            cb();
        }
    });
}

function deleteData(db) {
    print('deleteData()')
    if (!db) {
        return;
    }

    db.transaction( function(tx) {
        print('... delete user object')
        tx.executeSql('DELETE FROM user WHERE id=1');
    });
}
