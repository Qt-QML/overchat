#include "messagelistobject.h"

MessageListObject::MessageListObject(QObject *parent) : QObject(parent) {

}

MessageListObject::MessageListObject(QString author_id, QString text, bool is_author, QString attachment) {
    this->m_author_id = author_id;
    this->m_text = text;
    this->m_is_author = is_author;
    this->m_attachment = attachment;
}

QString MessageListObject::authorId() {
    return m_author_id;
}

QString MessageListObject::text() {
    return m_text;
}

QString MessageListObject::attachment() {
    return m_attachment;
}

bool MessageListObject::isAuthor() {
    return m_is_author;
}
