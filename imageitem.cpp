#include "ImageItem.h"

#include "firebase.h"
#include <QObject>
#include <QJsonObject>

ImageItem::ImageItem(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    this->current_image = QImage(":/images/no_image.jpg");
}

void ImageItem::paint(QPainter *painter)
{
    QRectF bounding_rect = boundingRect();
    QImage scaled = this->current_image.scaledToHeight(bounding_rect.height());
    QPointF center = bounding_rect.center() - scaled.rect().center();

    if(center.x() < 0)
        center.setX(0);
    if(center.y() < 0)
        center.setY(0);
   painter->drawImage(center, scaled);
}

QImage ImageItem::image() const
{
    return this->current_image;
}

void ImageItem::setImage(const QString access_token, const QString uri)
{
    this->_storageGetImage(access_token, uri);
}

void ImageItem::_storageGetImage(QString access_token, QString uri) {
    if (access_token == "") {qWarning("WARN! No Access Token provided"); return;}
    if (uri == "") {qWarning("WARN! No URI provided"); return;}

    QJsonObject jsonObj;
    QJsonDocument uploadDoc(jsonObj);

    Firebase *fb = new Firebase(uri, "", false);
    fb->setValueWithAuth(uploadDoc, access_token, "GET", "");

    connect(fb, SIGNAL(eventResponseReadyWithContentType(QByteArray, QByteArray)), this, SLOT(_onStorageGetImageResponse(QByteArray, QByteArray)));
}

void ImageItem::_onStorageGetImageResponse(QByteArray response, QByteArray content_type) {
    QByteArray type = "";

    if (content_type == "image/jpeg") type = "JPEG";
    if (content_type == "image/png") type = "PNG";

    if (type == "") {
        qDebug() << response;

        qWarning("WARN! Error while downloading an image");
        return;
    }

    QImage image = QImage::fromData(response, type);

    this->current_image = image;
    update();

    emit imageChanged();
}
