#ifndef IMAGEITEM_H
#define IMAGEITEM_H
#include <QQuickPaintedItem>
#include <QQuickItem>
#include <QPainter>
#include <QImage>

class ImageItem : public QQuickPaintedItem
{
Q_OBJECT
    Q_PROPERTY(QImage image READ image NOTIFY imageChanged)
public:
    ImageItem(QQuickItem *parent = nullptr);

    Q_INVOKABLE void setImage(const QString access_token, const QString uri);

    void paint(QPainter *painter);
    QImage image() const;

signals:
    void imageChanged();

private:
    QImage current_image;

    void _storageGetImage(QString access_token, QString uri);

private slots:
    void _onStorageGetImageResponse(QByteArray, QByteArray);
};
#endif // IMAGEITEM_H
