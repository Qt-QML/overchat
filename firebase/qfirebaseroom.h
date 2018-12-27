#ifndef QFIREBASEROOM_H
#define QFIREBASEROOM_H

#include <QObject>
#include <QJsonObject>
#include <QFile>
#include "qfirebaseuser.h"

// Docs at:
// http://doc.qt.io/qt-5/qtqml-tutorials-extending-qml-example.html

/**
 * @brief Класс комнаты Firebase
 *
 * Можно использовать в нескольких параллельно работающих QML-объектов.
 *
 * Работает с комнатами, сообщениями, загрузкой изображений.
 */
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

    /** Подключение параметров аутентификации, инициализация **/
    Q_INVOKABLE void setAuthParams(QJsonObject auth_params);
    Q_INVOKABLE void setRoomId(QString room_id);

    /** Отправка и приём сообщений **/
    Q_INVOKABLE void sendMessage(QString text, QString file_path);
    Q_INVOKABLE void subscribeMessageList();

signals:
    /** Изменения данных комнаты **/
    void roomIdChanged();
    void messageListChanged(QString author_id, QString text, bool is_author, QString attachment);

    /** Успешная загрузка файла в хранилище **/
    void storageSuccessful(QString file_uri);

private:
    QString _roomId;                    // ID настоящей комнаты
    QList<QObject*> m_message_list;     // локальный список сообщений

    /** Данные, заимствованные из объекта класса User **/
    QString _localId;       // ID пользователя, от имени которого выполняются действия
    QString _accessToken;   // его токен доступа в Google OAuth 2.0 (для действий загрузки и выгрузки фото)
    QString _refreshToken;  // его refresh-токен
    QString _firebaseToken; // его Firebase-токен (для действий отправки и приёма сообщений)
    int     _expiresIn;     // время жизни токена доступа от Google

    /**
     * @brief Записать список сообщений в локальный список сообщений
     * @param messageList список сообщений из БД Realtime
     */
    void _setMessageList(const QJsonObject &messageList);

    /**
     * @brief Добавить сообщение в локальный список сообщений
     * @param id            ID сообщения
     * @param text          Текст сообщения
     * @param attachment    URL вложения (только фото)
     */
    void _addMessageListItem(QString id, QString text, QString attachment);

    /**
     * @brief Очистить локальный список сообщений
     */
    void _clearMessageList();

    /**
     * @brief Сохранить информацию о сообщении в БД Realtime
     * @param text      текст сообщения
     * @param file_uri  URL вложения (только фото)
     */
    void _rdbSaveMessage(QString text, QString file_uri="");

    /**
     * @brief Получить список сообщений комнаты из БД Realtime
     */
    void _rdbGetMessageList();

    /**
     * @brief Запустить прослушивание списка сообщений
     */
    void _rdbListenMessageList();

    /**
     * @brief Сохранить изображение в хранилище
     */
    void _storageSaveImage(QByteArray image, QString ext);

    /**
     * @brief Сгенерировать случайную строку (для хранилища)
     */
    static QString GetRandomString();


private slots:
    void _onRdbSaveMessageResponse(QByteArray);
    void _onRdbGetMessageListResponse(QByteArray);
    void _onRdbMessageListChange(QString);
    void _onStorageSaveImage(QByteArray);
};

#endif // QFIREBASEROOM_H
