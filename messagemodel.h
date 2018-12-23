#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QAbstractListModel>
#include <QStringList>

class Message
{
public:
    Message(const QString &author_id, const QString &text, bool &is_author);

    QString authorId()  const;
    QString text()      const;
    bool    isAuthor()  const;

private:
    QString m_author_id;
    QString m_text;
    bool    m_is_author;
};

class MessageModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum MessageRoles {
            AuthorIdRole = Qt::UserRole + 1,
            TextRole,
            IsAuthorRole
        };

    MessageModel(QObject *parent = 0);

    void addMessage(const Message &message);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex& child) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
    QList<Message> m_message_list;

};

#endif // MESSAGEMODEL_H
