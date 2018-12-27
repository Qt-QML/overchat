#include "qfirebaseroom.h"
#include "messagelistobject.h"
#include "firebase.h"

#include <QDebug>
#include <QDir>
#include <QQuickItem>
#include <QFileInfo>

// for filename randomizer
#include <QRegularExpression>
#include <QUuid>

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
    this->_firebaseToken = auth_params["firebaseToken"].toString();
    this->_expiresIn    = auth_params["expiresIn"].toInt();
}

void QFirebaseRoom::setRoomId(QString room_id) {
    this->_roomId = room_id;

    // TODO: Restrict any changes later

    emit roomIdChanged();
}

void QFirebaseRoom::sendMessage(QString text, QString file_path) {
    if (this->_roomId == "") {qWarning("WARN! Room ID was not provided");}
    if (this->_accessToken == "") {qWarning("WARN! User is not authenticated"); return;}

    file_path = file_path.replace("file://", "");

    if (file_path != "") {
        QFile file(file_path);
        QFileInfo info(file);

        if (!file.size()) {
            qWarning() << "WARN! File is empty";
            return;
        }

        if (file.open(QIODevice::ReadOnly)) {
            QByteArray ba = file.readAll();
//            qDebug() << "Successfully opened the file.";

            // save file to storage
            this->_storageSaveImage(ba, info.suffix());

            connect(this, &QFirebaseRoom::storageSuccessful, [this, text] (QString file_uri) {
                this->_rdbSaveMessage(text, file_uri);
            });

            return;

        } else {
            qDebug() << "Cannot open the file.";
        }
    }

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

void QFirebaseRoom::_rdbSaveMessage(QString text, QString file_uri) {
    if (this->_roomId == "") {qFatal("No Room Id in QFirebaseRoom instance");}
    if (this->_localId == "") {qFatal("No Local Id in QFirebaseRoom instance");}
    if (text == "") {qFatal("Empty Message Text Provided");}

    QJsonObject jsonObj;

    jsonObj["author"] = this->_localId;
    jsonObj["text"] = text;

    if (file_uri != "") {
        jsonObj["attachment"] = file_uri;
    }

    QJsonDocument uploadDoc(jsonObj);

    Firebase *fb = new Firebase(RDB_URI, "rooms/" + this->_roomId + "/messages.json");
//    fb->setValue(uploadDoc, "POST", "access_token=" + this->_accessToken);
    fb->setValue(uploadDoc, "POST", "auth=" + this->_firebaseToken);

    connect(fb, SIGNAL(eventResponseReady(QByteArray)), this, SLOT(_onRdbSaveMessageResponse(QByteArray)));
}

void QFirebaseRoom::_rdbGetMessageList() {
    if (this->_roomId == "") {qFatal("No Room Id in QFirebaseRoom instance");}
    if (this->_localId == "") {qFatal("No Local Id in QFirebaseRoom instance");}

    QJsonObject jsonObj;
    QJsonDocument uploadDoc(jsonObj);

    Firebase *fb = new Firebase(RDB_URI, "rooms/" + this->_roomId + "/messages.json");
//    fb->setValue(uploadDoc, "GET", "access_token=" + this->_accessToken);
    fb->setValue(uploadDoc, "GET", "auth=" + this->_firebaseToken);

    connect(fb, SIGNAL(eventResponseReady(QByteArray)), this, SLOT(_onRdbGetMessageListResponse(QByteArray)));
}

void QFirebaseRoom::_rdbListenMessageList() {
    Firebase *fb = new Firebase(RDB_URI, "rooms/" + this->_roomId + "/messages.json");

    fb->listenEvents();

    connect(fb, SIGNAL(eventDataChanged(QString)), this, SLOT(_onRdbMessageListChange(QString)));
}

/**
 * ==============================================================
 *                          STORAGE QUERIES
 * ==============================================================
 */

void QFirebaseRoom::_storageSaveImage(QByteArray image, QString ext) {
    if (this->_roomId == "") {qFatal("No Room Id in QFirebaseRoom instance");}
    if (this->_localId == "") {qFatal("No Local Id in QFirebaseRoom instance");}

    QString content_type = "";

    if (ext == "jpg" || ext == "jpeg") content_type = "image/jpeg";
    if (ext == "png") content_type = "image/png";

    if (content_type == "") {
        qWarning("WARN! Unsupported extension, aborting.");
        return;
    }

    QString name = QFirebaseRoom::GetRandomString();

    Firebase *fb = new Firebase(STORAGE_UPLOAD_URI, "o");
    fb->setValue(image, content_type, "POST", "upload_type=media&name=" + name + "&access_token=" + this->_accessToken);

    connect(fb, SIGNAL(eventResponseReady(QByteArray)), this, SLOT(_onStorageSaveImage(QByteArray)));
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
//        qDebug() << key;
//        qDebug() << subobj;

        if (subobj.contains("text") && subobj.contains("author")) {
            QString attachment = "";

            if (subobj.contains("attachment")) {
                attachment = subobj["attachment"].toString();
            }

            this->_addMessageListItem(subobj["author"].toString(), subobj["text"].toString(), attachment);
            i++;
        }
    }
}

void QFirebaseRoom::_addMessageListItem(QString author_id, QString text, QString attachment) {
    bool is_author = (author_id == this->_localId);

//    qDebug() << "lid " << this->_localId << " id " << author_id << "ATT" << attachment;

    this->m_message_list.push_back(new MessageListObject(author_id, text, is_author, attachment));

    emit messageListChanged(author_id, text, is_author, attachment);
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
//    qDebug() << "_onRdbSaveMessageResponse";

    QJsonDocument document = QJsonDocument::fromJson(response);
    QJsonObject obj = document.object();

    if (obj.contains("error")) {
        qDebug() << obj;
        return;
    }
}

void QFirebaseRoom::_onRdbGetMessageListResponse(QByteArray response) {
//    qDebug() << "_onRdbGetMessageListResponse";

    QJsonDocument document = QJsonDocument::fromJson(response);
    QJsonObject obj = document.object();

    if (obj.contains("error")) {
        qDebug() << obj;
        return;
    }

    this->_setMessageList(obj);
}

void QFirebaseRoom::_onRdbMessageListChange(QString database_update) {
//    qDebug() << "_onRdbMessageListChange";

//    qDebug() << database_update;

    // remove service wrap chars of response
    QString sub_string = database_update.mid(6, database_update.length() - 1);

    QJsonDocument document = QJsonDocument::fromJson(sub_string.toUtf8());
    QJsonObject obj = document.object();

//    qDebug() << "OBJ " << obj;

    if (obj.contains("error")) {
        qDebug() << obj;
        return;
    }

    QJsonObject data = obj["data"].toObject();

    QString _id, id, text, author_id, attachment;

    _id = data["path"].toString();
    author_id = data["author"].toString();
    text = data["text"].toString();

    if (data.contains("attachment")) {
        attachment = data["attachment"].toString();
    } else {
        attachment = "";
    }

    id = _id.mid(1, _id.length() - 1);

    this->_addMessageListItem(author_id, text, attachment);
}

/**
 * ==============================================================
 *                  STORAGE RESPONSE HANDLERS
 * ==============================================================
 */


void QFirebaseRoom::_onStorageSaveImage(QByteArray response) {
//    qDebug() << "_onStorageSaveImage";

    QJsonDocument document = QJsonDocument::fromJson(response);
    QJsonObject obj = document.object();

//    qDebug() << obj;

    if (obj.contains("error")) {
        qDebug() << obj;
        return;
    }

    emit storageSuccessful(obj["mediaLink"].toString());
}

QString QFirebaseRoom::GetRandomString()
{
    QString str = QUuid::createUuid().toString();
    str.remove(QRegularExpression("{|}|-"));

    return str;
}

