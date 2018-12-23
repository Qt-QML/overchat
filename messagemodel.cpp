#include "messagemodel.h"

Message::Message(const QString &author_id, const QString &text, bool &is_author) :
    m_author_id(author_id), m_text(text), m_is_author(is_author)
{
}

QString Message::authorId() const
{
    return m_author_id;
}

QString Message::text() const
{
    return m_text;
}

bool Message::isAuthor() const {
    return m_is_author;
}

void MessageModel::addMessage(const Message &message) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_message_list << message;
    endInsertRows();
}

int MessageModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_message_list.count();
}

int MessageModel::columnCount(const QModelIndex &parent) const {
     Q_UNUSED(parent);
     return 3;
}

QModelIndex MessageModel::index(int row, int column, const QModelIndex &parent) const {
    return QModelIndex();
}

QModelIndex MessageModel::parent(const QModelIndex& child) const {
    return QModelIndex();
}

QVariant MessageModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_message_list.count())
        return QVariant();

    const Message &message = m_message_list[index.row()];
    if (role == AuthorIdRole)
        return message.authorId();
    else if (role == TextRole)
        return message.text();
    else if (role == IsAuthorRole)
        return message.isAuthor();

    return QVariant();
}
