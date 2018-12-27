#include "ioswebview.h"

#include <QtGui/QGuiApplication>
#include <QtGui/qpa/qplatformnativeinterface.h>

#include <UIKit/UIKit.h>

@interface QtWebViewDelegate : NSObject<UIWebViewDelegate> {
    IOSWebView *qIosWebViewPrivate;
}
- (QtWebViewDelegate *)initWithQWebViewPrivate:(IOSWebView *)webViewPrivate;
- (void)pageDone;

// protocol:
- (void)webViewDidStartLoad:(UIWebView *)webView;
- (void)webViewDidFinishLoad:(UIWebView *)webView;
- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error;
@end

@implementation QtWebViewDelegate
- (QtWebViewDelegate *)initWithQWebViewPrivate:(IOSWebView *)webViewPrivate
{
    qIosWebViewPrivate = webViewPrivate;
    return self;
}

- (void)pageDone
{
    qDebug() << "PDONE";
//    Q_EMIT qIosWebViewPrivate->pageFinished(qIosWebViewPrivate->getUrl());
}

- (void)webViewDidStartLoad:(UIWebView *)webView
{
    Q_UNUSED(webView);
}

- (void)webViewDidFinishLoad:(UIWebView *)webView
{
    [webView stopLoading];

    Q_UNUSED(webView);
//    qDebug() << "PFINISH";

    Q_EMIT qIosWebViewPrivate->pageFinished(qIosWebViewPrivate->getUrl());
}

- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
    qDebug() << "DFLWE:" << error.code << error.localizedDescription;

    Q_EMIT qIosWebViewPrivate->pageError(error.code, "UIWebView error", qIosWebViewPrivate->getUrl());

    Q_UNUSED(webView);
    Q_UNUSED(error);
}
@end

IOSWebView::IOSWebView(QQuickItem *parent) :
    QQuickItem(parent)
{
    setFlag(QQuickItem::ItemHasContents);
    pWebView = NULL;

    connect(this, SIGNAL(updateWebViewSize()), this, SLOT(onUpdateWebViewSize()));
//    connect(this, SIGNAL(urlChanged(QString)), this, SLOT(onUrlChanged(QString)));

    NSDictionary *dictionary = @{@"UserAgent": @"Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36"};
    [[NSUserDefaults standardUserDefaults] registerDefaults:dictionary];
    [[NSUserDefaults standardUserDefaults] synchronize];

    [[NSURLCache sharedURLCache] removeAllCachedResponses];
}

IOSWebView::~IOSWebView()
{
    if (pWebView) {
        [(UIWebView*)pWebView removeFromSuperview];
        [(UIWebView*)pWebView release];
        pWebView = NULL;
    }
}
QString IOSWebView::getUrl() {
    NSURL *url = [[pWebView request] URL];
    return QUrl::fromNSURL(url).toString();
}

void IOSWebView::setUrl(QString url) {
    this->onUrlChanged(url);
}

void IOSWebView::componentComplete()
{
    QQuickItem::componentComplete();
    if (pWebView == NULL) {
        UIView *pMainView = static_cast<UIView*>(QGuiApplication::platformNativeInterface()->nativeResourceForWindow("uiview", (QWindow*)window()));
        pWebView = [[UIWebView alloc] initWithFrame:CGRectMake(0, 0, 0, 0)];
        [pMainView addSubview:(UIWebView*)pWebView];
        ((UIWebView*)pWebView).scalesPageToFit = YES;
//        onUrlChanged(url);

        ((UIWebView*)pWebView).delegate = [[QtWebViewDelegate alloc] initWithQWebViewPrivate:this];
    }
}

void IOSWebView::itemChange(ItemChange change, const ItemChangeData & value)
{
    if (change == QQuickItem::ItemVisibleHasChanged && pWebView)
        ((UIWebView*)pWebView).hidden = !value.boolValue;
    QQuickItem::itemChange(change, value);
}


QSGNode* IOSWebView::updatePaintNode(QSGNode *pNode, UpdatePaintNodeData*)
{
    qmlRect = QRectF(absoluteQMLPosition(), QSizeF(width(), height()));
    emit updateWebViewSize();
    return pNode;
}

void IOSWebView::onUpdateWebViewSize()
{
    if (pWebView)
        ((UIWebView*)pWebView).frame = CGRectMake(qmlRect.x(), qmlRect.y(), qmlRect.width(), qmlRect.height());
}

void IOSWebView::onUrlChanged(QString newUrl)
{
    if (pWebView) {
        NSURL *url = [NSURL URLWithString:newUrl.toNSString()];
        NSURLRequest *requestObj = [NSURLRequest requestWithURL:url];
        [(UIWebView*)pWebView loadRequest:requestObj];
    }
}


QPointF IOSWebView::absoluteQMLPosition() {
    QPointF p(0, 0);
    QQuickItem* pItem = this;
    while (pItem != NULL) { // absolute position relative to rootItem
        p += pItem->position();
        pItem = pItem->parentItem();
    }
    return p;
}
