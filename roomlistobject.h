#ifndef ROOMLISTOBJECT_H
#define ROOMLISTOBJECT_H

#include <QObject>

class RoomListObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString id READ id NOTIFY idChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)

public:
    explicit RoomListObject(QObject *parent = nullptr);
    RoomListObject(QString id, QString name);

    QString id();
    QString name();
signals:

    void idChanged();
    void nameChanged();

private:
    QString m_id;
    QString m_name;
};

#endif // ROOMLISTOBJECT_H
