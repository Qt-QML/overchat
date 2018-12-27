#include "userlistobject.h"

UserListObject::UserListObject(QObject *parent) : QObject(parent) {

}

UserListObject::UserListObject(QString id, QString name) {
    this->m_id = id;
    this->m_name = name;
}

QString UserListObject::id() {
    return m_id;
}

QString UserListObject::name() {
    return m_name;
}
