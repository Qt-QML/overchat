#include "qfirebaseroom.h"
#include "messagelistobject.h"
#include "firebase.h"

#include <QDebug>
#include <QQuickItem>

QFirebaseRoom::QFirebaseRoom(QObject *parent) : QObject(parent) {

}

/**
 * ==============================================================
 *                        PROPERTY GETTERS
 * ==============================================================
 */

QString QFirebaseRoom::roomId() {
    return _roomId;
}

QList<QObject*> QFirebaseRoom::messageList() {
    return m_message_list;
}

/**
 * ==============================================================
 *                         PUBLIC ACTIONS
 * ==============================================================
 */

void QFirebaseRoom::setAuthParams(QJsonObject auth_params) {
    this->_localId      = auth_params["localId"].toString();
    this->_accessToken  = auth_params["idToken"].toString();
    this->_refreshToken = auth_params["refreshToken"].toString();
    this->_expiresIn    = auth_params["expiresIn"].toInt();
}

void QFirebaseRoom::setRoomId(QString room_id) {
    qDebug() << "sRI: " << room_id;

    this->_roomId = room_id;

    // TODO: Restrict any changes later

    emit roomIdChanged();
}

void QFirebaseRoom::sendMessage(QString text) {
    if (this->_roomId == "") {qWarning("WARN! Room ID was not provided");}
    if (this->_accessToken == "") {qWarning("WARN! User is not authenticated"); return;}

    this->_rdbSaveMessage(text);
}

void QFirebaseRoom::subscribeMessageList() {
    if (this->_roomId == "") {qWarning("WARN! Room ID was not provided");}
    if (this->_accessToken == "") {qWarning("WARN! User is not authenticated"); return;}

    this->_rdbListenMessageList();
    this->_rdbGetMessageList();
}

/**
 * ==============================================================
 *                    REMOTE DATABASE QUERIES
 * ==============================================================
 */

void QFirebaseRoom::_rdbSaveMessage(QString text) {
    if (this->_roomId == "") {qFatal("No Room Id in QFirebaseRoom instance");}
    if (this->_localId == "") {qFatal("No Local Id in QFirebaseRoom instance");}
    if (text == "") {qFatal("Empty Message Text Provided");}

    QJsonObject jsonObj;

    jsonObj["author"] = this->_localId;
    jsonObj["text"] = text;

    QJsonDocument uploadDoc(jsonObj);

    Firebase *fb = new Firebase(RDB_URI, "rooms/" + this->_roomId + "/messages.json");
    fb->setValue(uploadDoc, "POST", "access_token=" + this->_accessToken);

    connect(fb, SIGNAL(eventResponseReady(QByteArray)), this, SLOT(_onRdbSaveMessageResponse(QByteArray)));
}

void QFirebaseRoom::_rdbGetMessageList() {
    if (this->_roomId == "") {qFatal("No Room Id in QFirebaseRoom instance");}
    if (this->_localId == "") {qFatal("No Local Id in QFirebaseRoom instance");}

    QJsonObject jsonObj;
    QJsonDocument uploadDoc(jsonObj);

    Firebase *fb = new Firebase(RDB_URI, "rooms/" + this->_roomId + "/messages.json");
    fb->setValue(uploadDoc, "GET", "access_token=" + this->_accessToken);

    connect(fb, SIGNAL(eventResponseReady(QByteArray)), this, SLOT(_onRdbGetMessageListResponse(QByteArray)));
}

void QFirebaseRoom::_rdbListenMessageList() {
    Firebase *fb = new Firebase(RDB_URI, "rooms/" + this->_roomId + "/messages.json");

    fb->listenEvents();

    connect(fb, SIGNAL(eventDataChanged(QString)), this, SLOT(_onRdbMessageListChange(QString)));
}

/**
 * ==============================================================
 *                        PROPERTY SETTERS
 * ==============================================================
 */

void QFirebaseRoom::_setMessageList(const QJsonObject &room_list) {
    int i = 0;

    this->_clearMessageList();

    foreach (const QString& key, room_list.keys()) {
        QJsonObject subobj = room_list.value(key).toObject();
        qDebug() << key;
        qDebug() << subobj;

        if (subobj.contains("text") && subobj.contains("author")) {
            this->_addMessageListItem(subobj["author"].toString(), subobj["text"].toString());
            i++;
        }
    }
}

void QFirebaseRoom::_addMessageListItem(QString author_id, QString text) {
    bool is_author = (author_id == this->_localId);

    qDebug() << "lid " << this->_localId << " id " << author_id;

    this->m_message_list.push_back(new MessageListObject(author_id, text, is_author));

    emit messageListChanged(author_id, text, is_author);
}

void QFirebaseRoom::_clearMessageList() {
    this->m_message_list.clear();
}

/**
 * ==============================================================
 *               REMOTE DATABASE RESPONSE HANDLERS
 * ==============================================================
 */

void QFirebaseRoom::_onRdbSaveMessageResponse(QByteArray response) {
    qDebug() << "_onRdbSaveMessageResponse";

    QJsonDocument document = QJsonDocument::fromJson(response);
    QJsonObject obj = document.object();

    qDebug() << obj;
}

void QFirebaseRoom::_onRdbGetMessageListResponse(QByteArray response) {
    qDebug() << "_onRdbGetMessageListResponse";

    QJsonDocument document = QJsonDocument::fromJson(response);
    QJsonObject obj = document.object();

    qDebug() << obj;

    this->_setMessageList(obj);
}

void QFirebaseRoom::_onRdbMessageListChange(QString database_update) {
    qDebug() << "_onRdbMessageListChange";

    qDebug() << database_update;

    // remove service wrap chars of response
    QString sub_string = database_update.mid(6, database_update.length() - 1);

    QJsonDocument document = QJsonDocument::fromJson(sub_string.toUtf8());
    QJsonObject obj = document.object();

    qDebug() << "OBJ " << obj;

    QJsonObject data = obj["data"].toObject();

    QString _id, id, text, author_id;

    _id = data["path"].toString();
    author_id = data["author"].toString();
    text = data["text"].toString();

    id = _id.mid(1, _id.length() - 1);

    this->_addMessageListItem(author_id, text);
}

