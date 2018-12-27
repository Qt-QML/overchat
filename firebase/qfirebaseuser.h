#ifndef FIREBASE_H
#define FIREBASE_H

#include <QObject>
#include <QJsonObject>

// Docs at:
// http://doc.qt.io/qt-5/qtqml-tutorials-extending-qml-example.html

/**
 * @brief Класс пользователя Firebase.
 *
 * Использовать в качестве одиночного QML-объекта.
 *
 * Работает с аутентификацией и личными данными пользователя.
 */
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

    /** Возвратить аутентификационные параметры **/
    Q_INVOKABLE QJsonObject getAuthParams();

    /** Аутентификация, регистрация, выход **/
    Q_INVOKABLE void        signinOauth(QString access_token, QString refresh_token, int expires_in = -1);
    Q_INVOKABLE void        signupOauth(QString access_token, QString refresh_token, int expires_in = -1);
    Q_INVOKABLE void        signout();

    /** Выгрузка, загрузка, прослушивание **/
    Q_INVOKABLE void        updateUserList();
    Q_INVOKABLE void        updateRoomList();
    Q_INVOKABLE void        createRoom(QString user_id, QString room_name);
    Q_INVOKABLE void        subscribeRoomList();

signals:
    /** Изменения данных пользователя **/
    void emailChanged();
    void nameChanged();
    void roomListChanged();

    /** Изменения списка пользователей **/
    void userListChanged();

    /** Успешная аутентификация **/
    void signinCompleted(QString, QJsonObject);

private:
    QString         m_email;        // почта
    QString         m_name;         // имя пользователя
    QList<QObject*> m_user_list;    // локальный список пользователей
    QList<QObject*> m_room_list;    // локальный список комнат

    QString _localId;               // ID пользователя в Firebase
    QString _accessToken;           // токен доступа от Google OAuth 2.0
    QString _refreshToken;          // refresh-токен от Google OAuth 2.0
    QString _firebaseToken;         // токен доступа от Firebase
    int     _expiresIn;             // время жизни токена доступа от Google

    void _setEmail(const QString &email);
    void _setName(const QString &name);

    /**
     * @brief Записать список пользователей в локальный список пользователей
     * @param userList список пользователей из БД Realtime
     */
    void _setUserList(const QJsonObject &userList);

    /**
     * @brief Записать список комнат в локальный список комнат
     * @param roomList список комнат из БД Realtime
     */
    void _setRoomList(const QJsonObject &roomList);

    /**
     * @brief Добавить пользователя в локальный список пользователей
     */
    void _addUserListItem(QString id, QString name);

    /**
     * @brief Добавить комнату в локальный список комнат
     */
    void _addRoomListItem(QString id, QString name);

    /**
     * @brief Очистить локальный список пользователей
     */
    void _clearUserList();

    /**
     * @brief Очистить список комнат
     */
    void _clearRoomList();

    /**
     * @brief Выполнить аутентификацию в Firebase с помощью токена OAuth 2.0
     */
    void _authOauth(QString access_token, QString refresh_token, int expires_in = -1);

    /**
     * @brief Отозвать refresh-токен
     */
    void _authRevokeRefreshToken();

    /**
     * @brief Получить глобальный список пользователей из БД Realtime
     */
    void _rdbGetUserList();

    /**
     * @brief Получить список комнат пользователя из БД Realtime
     */
    void _rdbGetRoomList();

    /**
     * @brief Сохранить информацию о пользователе в БД Realtime
     *
     * Если пользователь был сохранён ранее, данный запрос выполнится с ошибкой.
     */
    void _rdbSaveUserInfo();

    /**
     * @brief Сохранить комнату в БД Realtime
     *
     * @param user_id   id собеседника
     * @param room_name название комнаты (отображаемое для создателя)
     *
     * Название комнаты для собеседника будет сгенерировано на основе
     * имени создателя
     */
    void _rdbSaveRoom(QString user_id, QString room_name);

    /**
     * @brief Сохранить ссылку на комнату в объекте пользователя в БД Realtime
     * @param user_id       id пользователя
     * @param room_id       id комнаты
     * @param room_name     название комнаты
     */
    void _rdbSaveRoomReference(QString user_id, QString room_id, QString room_name);

    /**
     * @brief Запустить прослушивание списка комнат
     */
    void _rdbListenRoomList();

private slots:
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
