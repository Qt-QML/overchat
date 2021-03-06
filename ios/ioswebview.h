#ifndef IOSWEBVIEW_H
#define IOSWEBVIEW_H

#include <QQuickItem>

/**
 * @brief Обёртка Objective-C компонента для просмотра веб-страниц
 *
 * Для генерации событий загрузки страницы используется интерфейс QtWebViewDelegate.
 */
class IOSWebView : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString url MEMBER url NOTIFY urlChanged)
public:
    explicit IOSWebView(QQuickItem *parent = 0);
    ~IOSWebView();

    Q_INVOKABLE QString getUrl();
    Q_INVOKABLE void    setUrl(QString url);

protected:
    QSGNode* updatePaintNode(QSGNode *pNode, UpdatePaintNodeData*);
    virtual void componentComplete();
    virtual void itemChange(ItemChange change, const ItemChangeData & value);

private:
    QPointF absoluteQMLPosition();

    void* pWebView;

    QString url;
    QRectF qmlRect;


signals:
    void updateWebViewSize();
    void urlChanged(QString newUrl);
    void pageFinished(QString url);
    void pageError(int code, QString error, QString url);

private slots:
    void onUpdateWebViewSize();
    void onUrlChanged(QString newUrl);

};
#endif // IOSWEBVIEW_H
