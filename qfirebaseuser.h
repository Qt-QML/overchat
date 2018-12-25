#ifndef FIREBASE_H
#define FIREBASE_H

#include <QObject>
#include <QJsonObject>

// Docs at:
// http://doc.qt.io/qt-5/qtqml-tutorials-extending-qml-example.html

class QFirebaseUser : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString          email       READ email          NOTIFY emailChanged)
    Q_PROPERTY(QString          name        READ name           NOTIFY nameChanged)
    Q_PROPERTY(QList<QObject*>  userList    READ userList       NOTIFY userListChanged)
    Q_PROPERTY(QList<QObject*>  roomList    READ roomList       NOTIFY roomListChanged)

public:
    explicit QFirebaseUser(QObject *parent = nullptr);

    const QString RESSTAT_SUCCESS   = "succ";
    const QString RESSTAT_FAIL      = "fail";

    QString email();
    QString name();
    QList<QObject*> userList();
    QList<QObject*> roomList();

    Q_INVOKABLE QJsonObject getAuthParams();

//    Q_INVOKABLE void        signin(QString email, QString password);
//    Q_INVOKABLE void        signup(QString email, QString password, QString name);

    Q_INVOKABLE void        signinOauth(QString access_token, QString refresh_token, int expires_in = -1);
    Q_INVOKABLE void        signupOauth(QString access_token, QString refresh_token, int expires_in = -1);
//    Q_INVOKABLE void        signinByAuthParams(QJsonObject auth_params);
    Q_INVOKABLE void        signout();

    Q_INVOKABLE void        updateUserList();
    Q_INVOKABLE void        updateRoomList();
    Q_INVOKABLE void        createRoom(QString user_id, QString room_name);
    Q_INVOKABLE void        subscribeRoomList();

signals:
    void emailChanged();
    void nameChanged();
    void userListChanged();
    void roomListChanged();

    void signinCompleted(QString, QJsonObject);

private:
    QString         m_email;
    QString         m_name;
    QList<QObject*> m_user_list;
    QList<QObject*> m_room_list;

    QString _possibleEmail;
    QString _possibleName;

    QString _localId;
    QString _accessToken;
    QString _refreshToken;
    int     _expiresIn;

    void _setEmail(const QString &email);
    void _setName(const QString &name);
    void _setUserList(const QJsonObject &userList);
    void _setRoomList(const QJsonObject &roomList);

    void _addUserListItem(QString id, QString name);
    void _addRoomListItem(QString id, QString name);

    void _clearUserList();
    void _clearRoomList();

//    void _authMethod(QString email, QString password, QString methodName, QString displayName = "");
//    void _refresh(QString refresh_token);
    void _authOauth(QString access_token, QString refresh_token, int expires_in = -1);
    void _authRevokeRefreshToken();

    void _rdbGetUserList();
    void _rdbGetRoomList();
    void _rdbSaveUserInfo();
    void _rdbSaveRoom(QString user_id, QString room_name);
    void _rdbSaveRoomReference(QString user_id, QString room_id, QString room_name);

    void _rdbListenRoomList();


private slots:
//    void _onRefreshResponse(QByteArray);
//    void _onAuthCompleted(QString, QJsonObject);

    void _onOauthResponse(QByteArray);
    void _onOauthCompleted(QString, QJsonObject);
    void _onOauthRevokeRefreshToken(QByteArray);

    void _onRdbGetUserListResponse(QByteArray);
    void _onRdbGetRoomListResponse(QByteArray);
    void _onRdbSaveUserInfoResponse(QByteArray);
    void _onRdbSaveRoomResponse(QByteArray, QString, QString);
    void _onRdbSaveRoomReferenceResponse(QByteArray);
    void _onRdbRoomListChange(QString);
};

Q_DECLARE_METATYPE(QFirebaseUser*)

#endif // FIREBASE_H
