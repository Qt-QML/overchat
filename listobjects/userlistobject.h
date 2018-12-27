#ifndef USERLISTOBJECT_H
#define USERLISTOBJECT_H

#include <QObject>

/**
 * @brief Элемент списка пользователей
 */
class UserListObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString id READ id NOTIFY idChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)

public:
    explicit UserListObject(QObject *parent = nullptr);
    UserListObject(QString id, QString name);

    QString id();
    QString name();
signals:

    void idChanged();
    void nameChanged();

private:
    QString m_id;
    QString m_name;
};

#endif // USERLISTOBJECT_H
