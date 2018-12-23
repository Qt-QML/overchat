#ifndef QFIREBASEROOM_H
#define QFIREBASEROOM_H

#include <QObject>
#include <QJsonObject>
#include <qfirebaseuser.h>

// Docs at:
// http://doc.qt.io/qt-5/qtqml-tutorials-extending-qml-example.html

class QFirebaseRoom : public QAbstractListModel
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

    Q_INVOKABLE void subscribeMessageList();

signals:
    void roomIdChanged();
    void messageListChanged();

private:
    QString _roomId;

    QString _localId;
    QString _accessToken;
    QString _refreshToken;
    int     _expiresIn;

    QList<QObject*> m_message_list;

    void _setMessageList(const QJsonObject &messageList);

    void _addMessageListItem(QString id, QString text);

    void _clearMessageList();

    void _rdbGetMessageList();
    void _rdbListenMessageList();


private slots:
    void _onRdbGetMessageListResponse(QByteArray);
    void _onRdbMessageListChange(QString);
};

#endif // QFIREBASEROOM_H
