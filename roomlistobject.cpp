#include "roomlistobject.h"

RoomListObject::RoomListObject(QObject *parent) : QObject(parent) {

}

RoomListObject::RoomListObject(QString id, QString name) {
    this->m_id = id;
    this->m_name = name;
}

QString RoomListObject::id() {
    return m_id;
}

QString RoomListObject::name() {
    return m_name;
}
