#include "messagelistobject.h"

MessageListObject::MessageListObject(QObject *parent) : QObject(parent) {

}

MessageListObject::MessageListObject(QString author_id, QString text, bool is_author) {
    this->m_author_id = author_id;
    this->m_text = text;
    this->m_is_author = is_author;
}

QString MessageListObject::authorId() {
    return m_author_id;
}

QString MessageListObject::text() {
    return m_text;
}

bool MessageListObject::isAuthor() {
    return m_is_author;
}
