#ifndef QFIREBASEROOM_H
#define QFIREBASEROOM_H

#include <QObject>
#include <QJsonObject>
#include <QFile>
#include <qfirebaseuser.h>

// Docs at:
// http://doc.qt.io/qt-5/qtqml-tutorials-extending-qml-example.html

class QFirebaseRoom : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QJsonObject auth_params)
    Q_PROPERTY(QString roomId               READ roomId)
    Q_PROPERTY(QList<QObject*> messageList  READ messageList NOTIFY messageListChanged)

public:
    explicit QFirebaseRoom(QObject *parent = nullptr);

    QString roomId();
    QList<QObject*> messageList();

    Q_INVOKABLE void setAuthParams(QJsonObject auth_params);
    Q_INVOKABLE void setRoomId(QString room_id);

    Q_INVOKABLE void sendMessage(QString text, QString file_path);
    Q_INVOKABLE void subscribeMessageList();

signals:
    void roomIdChanged();
    void messageListChanged(QString author_id, QString text, bool is_author, QString attachment);
    void storageSuccessful(QString file_uri);

private:
    QString _roomId;

    QString _localId;
    QString _accessToken;
    QString _refreshToken;
    QString _firebaseToken;
    int     _expiresIn;

    QList<QObject*> m_message_list;

    void _setMessageList(const QJsonObject &messageList);

    void _addMessageListItem(QString id, QString text, QString attachment);

    void _clearMessageList();

    void _rdbSaveMessage(QString text, QString file_uri="");
    void _rdbGetMessageList();
    void _rdbListenMessageList();

    void _storageSaveImage(QByteArray image, QString ext);

    static QString GetRandomString();


private slots:
    void _onRdbSaveMessageResponse(QByteArray);
    void _onRdbGetMessageListResponse(QByteArray);
    void _onRdbMessageListChange(QString);
    void _onStorageSaveImage(QByteArray);
};

#endif // QFIREBASEROOM_H
