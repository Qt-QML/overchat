#ifndef MESSAGELISTOBJECT_H
#define MESSAGELISTOBJECT_H

#include <QObject>

/**
 * @brief Элемент списка сообщений
 */
class MessageListObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString  authorId    READ authorId   NOTIFY authorIdChanged)
    Q_PROPERTY(QString  text        READ text       NOTIFY textChanged)
    Q_PROPERTY(QString  attachment  READ attachment NOTIFY attachmentChanged)
    Q_PROPERTY(bool     isAuthor    READ isAuthor   NOTIFY isAuthorChanged)

public:
    explicit MessageListObject(QObject *parent = nullptr);
    MessageListObject(QString author_id, QString text, bool is_author, QString attachment);

    QString authorId();
    QString text();
    QString attachment();
    bool isAuthor();

signals:
    void authorIdChanged();
    void textChanged();
    void isAuthorChanged();
    void attachmentChanged();

private:
    QString m_author_id;
    QString m_text;
    QString m_attachment;
    bool    m_is_author;
};

#endif // MESSAGELISTOBJECT_H
