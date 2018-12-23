#include <QSignalMapper>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "qfirebaseuser.h"
#include "userlistobject.h"
#include "roomlistobject.h"
#include "firebase.h"

QFirebaseUser::QFirebaseUser(QObject *parent) : QObject(parent) {
    this->m_email       = "";
    this->m_name        = "";

    this->_localId      = "";
    this->_accessToken  = "";
    this->_refreshToken = "";
    this->_expiresIn    = -1;

    this->_possibleEmail = "";
}

/**
 * ==============================================================
 *                        PROPERTY GETTERS
 * ==============================================================
 */

QString QFirebaseUser::email() {
    return m_email;
}

QString QFirebaseUser::name() {
    return m_name;
}

QList<QObject*> QFirebaseUser::userList() {
    return m_user_list;
}

QList<QObject*> QFirebaseUser::roomList() {
    return m_room_list;
}

/**
 * ==============================================================
 *                         PUBLIC ACTIONS
 * ==============================================================
 */

QJsonObject QFirebaseUser::getAuthParams() {
    QJsonObject jsonObj;

    if (this->m_email == "") {
        return jsonObj;
    }

    jsonObj["email"]        = this->m_email;
    jsonObj["displayName"]  = this->m_name;

    jsonObj["localId"]      = this->_localId;
    jsonObj["idToken"]      = this->_accessToken;
    jsonObj["refreshToken"] = this->_refreshToken;
    jsonObj["expiresIn"]    = this->_expiresIn;

    return jsonObj;
}

void QFirebaseUser::signin(QString email, QString password) {
    this->_authMethod(email, password, "relyingparty/verifyPassword");
}

void QFirebaseUser::signup(QString email, QString password, QString name) {
    this->_authMethod(email, password, "relyingparty/signupNewUser", name);
    connect(this, SIGNAL(signinCompleted(QString, QJsonObject, bool)), this, SLOT(_onAuthCompleted(QString, QJsonObject, bool)));
}

void QFirebaseUser::signinByAuthParams(QJsonObject auth_params) {
    if (!auth_params.contains("email")) {return;}
    if (!auth_params.contains("refreshToken")) {return;}

    this->_possibleEmail    = auth_params["email"].toString();
    this->_possibleName     = auth_params["displayName"].toString();

    this->_refresh(auth_params["refreshToken"].toString());
}

void QFirebaseUser::signout() {
    this->_localId       = "";
    this->_accessToken   = "";
    this->_refreshToken  = "";
    this->_expiresIn     = -1;

    this->_setEmail("");
    this->_setName("");

    this->_clearUserList(); emit userListChanged();
    this->_clearRoomList(); emit roomListChanged();
}

void QFirebaseUser::updateUserList() {
    if (this->_accessToken == "") {qWarning("WARN! User is not authenticated"); return;}
    this->_rdbGetUserList();
}

void QFirebaseUser::updateRoomList() {
    if (this->_accessToken == "") {qWarning("WARN! User is not authenticated"); return;}
    this->_rdbGetRoomList();
}

void QFirebaseUser::createRoom(QString user_id, QString room_name) {
    if (this->_accessToken == "") {qWarning("WARN! User is not authenticated"); return;}
    if (this->m_name == "") {qWarning("WARN! User's name is empty!"); return;}
    this->_rdbSaveRoom(user_id, room_name);
}

void QFirebaseUser::subscribeRoomList() {
    if (this->_accessToken == "") {qWarning("WARN! User is not authenticated"); return;}
    this->_rdbListenRoomList();

    this->_rdbGetRoomList();
}

/**
 * ==============================================================
 *                  AUTHENTICATION QUERIES
 * ==============================================================
 */

void QFirebaseUser::_refresh(QString refresh_token) {
    if (refresh_token.length() == 0) {qFatal("Token is empty");}

    QJsonObject jsonObj;

    jsonObj["grant_type"] = "refresh_token";
    jsonObj["refresh_token"] = refresh_token;

    QJsonDocument uploadDoc(jsonObj);

    Firebase *fb = new Firebase(REFRESH_URI, "token");
    fb->setValue(uploadDoc, "POST", "key=" + API_KEY);

    connect(fb, SIGNAL(eventResponseReady(QByteArray)), this, SLOT(_onAuthResponse(QByteArray)));
}

void QFirebaseUser::_authMethod(QString email, QString password, QString method_name, QString display_name) {
    if (email.length() == 0) {qFatal("Trying to signin with empty password");}
    if (password.length() == 0) {qFatal("Trying to signin with empty password");}

    QJsonObject jsonObj;

    if (display_name != "") {
        jsonObj["displayName"] = display_name;
    }

    jsonObj["email"] = email;
    jsonObj["password"] = password;
    jsonObj["returnSecureToken"] = true;

    QJsonDocument uploadDoc(jsonObj);

    Firebase *fb = new Firebase(AUTH_URI, method_name);
    fb->setValue(uploadDoc, "POST", "key=" + API_KEY);

    connect(fb, SIGNAL(eventResponseReady(QByteArray)), this, SLOT(_onAuthResponse(QByteArray)));
}

/**
 * ==============================================================
 *                    REMOTE DATABASE QUERIES
 * ==============================================================
 */

void QFirebaseUser::_rdbGetUserList() {
    if (this->_localId == "") {qFatal("No Local Id in QFirebase instance");}
    if (this->m_name == "") {qFatal("No User Name in QFirebase instance");}

    QJsonObject jsonObj;
    QJsonDocument uploadDoc(jsonObj);

    Firebase *fb = new Firebase(RDB_URI, "users.json");
    fb->setValue(uploadDoc, "GET", "auth=" + this->_accessToken);

    connect(fb, SIGNAL(eventResponseReady(QByteArray)), this, SLOT(_onRdbGetUserListResponse(QByteArray)));
}

void QFirebaseUser::_rdbGetRoomList() {
    if (this->_localId == "") {qFatal("No Local Id in QFirebase instance");}

    QJsonObject jsonObj;
    QJsonDocument uploadDoc(jsonObj);

    Firebase *fb = new Firebase(RDB_URI, "users/" + this->_localId + "/rooms.json");
    fb->setValue(uploadDoc, "GET", "auth=" + this->_accessToken);

    connect(fb, SIGNAL(eventResponseReady(QByteArray)), this, SLOT(_onRdbGetRoomListResponse(QByteArray)));
}

void QFirebaseUser::_rdbSaveUserInfo() {
    if (this->_localId == "") {qFatal("No Local Id in QFirebase instance");}
    if (this->m_name == "") {qFatal("No User Name in QFirebase instance");}

    QJsonObject jsonObj;

    jsonObj["name"] = this->m_name;

    QJsonDocument uploadDoc(jsonObj);

    qDebug() << jsonObj;
    qDebug() << uploadDoc;

    Firebase *fb = new Firebase(RDB_URI, "users/" + this->_localId + ".json");
    fb->setValue(uploadDoc, "PUT", "auth=" + this->_accessToken);

    connect(fb, SIGNAL(eventResponseReady(QByteArray)), this, SLOT(_onRdbSaveUserInfoResponse(QByteArray)));
}

/**
 * @brief Save Room to Realtime Database
 *
 * @param user_id   id of the opponent user
 * @param room_name name of the room that will appear for creator
 *
 * Name of the room for the opponent side will be generated from
 * creator's user name.
 */
void QFirebaseUser::_rdbSaveRoom(QString user_id, QString room_name) {
    if (this->_localId == "") {qFatal("No Local Id in QFirebase instance");}
    if (this->m_name == "") {qFatal("No User Name in QFirebase instance");}

    QJsonObject jsonObj;

    QJsonArray jsonMembers;

    if (this->_localId != user_id) {
        jsonMembers.append(this->_localId);
    }

    jsonMembers.append(user_id);

    jsonObj["members"] = jsonMembers;

    QJsonDocument uploadDoc(jsonObj);

    Firebase *fb = new Firebase(RDB_URI, "rooms.json");
    fb->setValue(uploadDoc, "POST", "auth=" + this->_accessToken);

    connect(fb, &Firebase::eventResponseReady, [this, user_id, room_name] (QByteArray arg) {
        _onRdbSaveRoomResponse(arg, user_id, room_name);
    });
}

void QFirebaseUser::_rdbSaveRoomReference(QString user_id, QString room_id, QString room_name) {
    if (this->_localId == "") {qFatal("No Local Id in QFirebase instance");}
    if (this->m_name == "") {qFatal("No User Name in QFirebase instance");}

    QJsonObject jsonObj;

    jsonObj["name"] = room_name;

    QJsonDocument uploadDoc(jsonObj);

    qDebug() << jsonObj;
    qDebug() << uploadDoc;

    qDebug() << "UID" << user_id << "RID" << room_id << "RNAM" << room_name;

    Firebase *fb = new Firebase(RDB_URI, "users/" + user_id + "/rooms/" + room_id +  ".json");
    fb->setValue(uploadDoc, "PUT", "auth=" + this->_accessToken);

    connect(fb, SIGNAL(eventResponseReady(QByteArray)), this, SLOT(_onRdbSaveRoomReferenceResponse(QByteArray)));
}

void QFirebaseUser::_rdbListenRoomList() {
    Firebase *fb = new Firebase(RDB_URI, "users/" + this->_localId + "/rooms.json");

    fb->listenEvents();

    connect(fb, SIGNAL(eventDataChanged(QString)), this, SLOT(_onRdbRoomListChange(QString)));
}

/**
 * ==============================================================
 *                        PROPERTY SETTERS
 * ==============================================================
 */

void QFirebaseUser::_setEmail(const QString &email) {
    if (email == m_email)
        return;

    m_email = email;
    emit emailChanged();
}

void QFirebaseUser::_setName(const QString &name) {
    if (name == m_name)
        return;

    m_name = name;
    emit nameChanged();
}

void QFirebaseUser::_setUserList(const QJsonObject &user_list) {
    int i = 0;

    this->_clearUserList();

    foreach (const QString& key, user_list.keys()) {
        QJsonObject subobj = user_list.value(key).toObject();
        qDebug() << key;
        qDebug() << subobj;

        if (subobj.contains("name")) {
            this->_addUserListItem(key, subobj["name"].toString());
            i++;
        }
    }

    if (i > 0) {
        emit userListChanged();
    }
}

void QFirebaseUser::_setRoomList(const QJsonObject &room_list) {
    int i = 0;

    this->_clearRoomList();

    foreach (const QString& key, room_list.keys()) {
        QJsonObject subobj = room_list.value(key).toObject();
        qDebug() << key;
        qDebug() << subobj;

        if (subobj.contains("name")) {
            this->_addRoomListItem(key, subobj["name"].toString());
            i++;
        }
    }

    if (i > 0) {
        emit roomListChanged();
    }
}

void QFirebaseUser::_addUserListItem(QString id, QString name) {
    this->m_user_list.push_back(new UserListObject(id, name));
}

void QFirebaseUser::_addRoomListItem(QString id, QString name) {
    this->m_room_list.push_back(new RoomListObject(id, name));
}

void QFirebaseUser::_clearUserList() {
    this->m_user_list.clear();
}

void QFirebaseUser::_clearRoomList() {
    this->m_room_list.clear();
}

/**
 * ==============================================================
 *               AUTHENTICATION RESPONSE HANDLERS
 * ==============================================================
 */

void QFirebaseUser::_onAuthResponse(QByteArray response) {
    qDebug() << "onAuthResponse";

    bool isRefresh  = false,
         isSignup   = false;

    QJsonDocument document = QJsonDocument::fromJson(response);
    QJsonObject obj = document.object();

    qDebug() << obj;

    if (obj.contains("error")) {
        emit signinCompleted(QFirebaseUser::RESSTAT_FAIL, obj, false);

        this->_possibleEmail = "";
        return;
    }

    if (obj.contains("kind")) {
        isRefresh = false;
        if (obj["kind"] == "identitytoolkit#SignupNewUserResponse") {
            isSignup = true;
        }
    } else {
        isRefresh = true;
    }

    this->_localId       = isRefresh ? obj["user_id"].toString()        : obj["localId"].toString();
    this->_accessToken   = isRefresh ? obj["id_token"].toString()       : obj["idToken"].toString();
    this->_refreshToken  = isRefresh ? obj["refresh_token"].toString()  : obj["refreshToken"].toString();
    this->_expiresIn     = isRefresh ? obj["expires_in"].toInt()        : obj["expiresIn"].toInt();

    QString new_email = this->_possibleEmail == ""  ? obj["email"].toString() : this->_possibleEmail;
    QString new_name  = this->_possibleName == ""   ? obj["displayName"].toString() : this->_possibleName;

    this->_setEmail(new_email);
    this->_setName(new_name);

    this->_possibleEmail = "";
    this->_possibleName = "";

    emit signinCompleted(QFirebaseUser::RESSTAT_SUCCESS, this->getAuthParams(), isSignup);
}

void QFirebaseUser::_onAuthCompleted(QString status, QJsonObject auth_params, bool is_signup) {
    if (status == QFirebaseUser::RESSTAT_SUCCESS && is_signup == true) {
        this->_rdbSaveUserInfo();
    }
}

/**
 * ==============================================================
 *               REMOTE DATABASE RESPONSE HANDLERS
 * ==============================================================
 */

void QFirebaseUser::_onRdbSaveUserInfoResponse(QByteArray response) {
    qDebug() << "_onRdbSaveUserInfoResponse";

    QJsonDocument document = QJsonDocument::fromJson(response);
    QJsonObject obj = document.object();

    qDebug() << obj;
}

void QFirebaseUser::_onRdbGetUserListResponse(QByteArray response) {
    qDebug() << "_onRdbGetUserListResponse";

    QJsonDocument document = QJsonDocument::fromJson(response);
    QJsonObject obj = document.object();

    qDebug() << obj;

    this->_setUserList(obj);
}

void QFirebaseUser::_onRdbGetRoomListResponse(QByteArray response) {
    qDebug() << "_onRdbGetRoomListResponse";

    QJsonDocument document = QJsonDocument::fromJson(response);
    QJsonObject obj = document.object();

    qDebug() << obj;

    this->_setRoomList(obj);
}

void QFirebaseUser::_onRdbSaveRoomResponse(QByteArray response, QString user_id, QString room_name) {
    qDebug() << "_onRdbSaveRoomResponse" << user_id;

    QJsonDocument document = QJsonDocument::fromJson(response);
    QJsonObject obj = document.object();

    qDebug() << obj;

    // TODO: Error handling

    QString room_id = obj["name"].toString();

    // Save Room to U1 as room_name
    this->_rdbSaveRoomReference(this->_localId, room_id, room_name);
    // Save Room to U2 as this->m_name
    this->_rdbSaveRoomReference(user_id,        room_id, this->m_name);
}

void QFirebaseUser::_onRdbSaveRoomReferenceResponse(QByteArray response) {
    QJsonDocument document = QJsonDocument::fromJson(response);
    QJsonObject obj = document.object();

    qDebug() << obj;
}

void QFirebaseUser::_onRdbRoomListChange(QString database_update) {
    qDebug() << "_onRdbRoomListChange";

    qDebug() << database_update;

    // remove service wrap chars of response
    QString sub_string = database_update.mid(6, database_update.length() - 1);

    QJsonDocument document = QJsonDocument::fromJson(sub_string.toUtf8());
    QJsonObject obj = document.object();

    qDebug() << "OBJ " << obj;

    QJsonObject data = obj["data"].toObject();

    QString _id, id, name;

    _id = data["path"].toString();
    name = data["name"].toString();

    id = _id.mid(1, _id.length() - 1);

    this->_addRoomListItem(id, name);

    emit roomListChanged();
}
