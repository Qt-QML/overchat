#include "qfirebase.h"

#include "firebase.h"
#include <QJsonObject>
#include <QJsonDocument>

const QString API_KEY = "AIzaSyAnxv6M_ALnWIzO4LpSsAFukER50gb3Umw";

const QString AUTH_URI      = "https://www.googleapis.com/identitytoolkit/v3";
const QString REFRESH_URI   = "https://securetoken.googleapis.com/v1";

QFirebase::QFirebase(QObject *parent) : QObject(parent) {
    this->m_email       = "";
    this->m_name        = "";

    this->_localId      = "";
    this->_accessToken  = "";
    this->_refreshToken = "";
    this->_expiresIn    = -1;

    this->_possibleEmail = "";
}

QString QFirebase::email() {
    return m_email;
}

QString QFirebase::name() {
    return m_name;
}

QJsonObject QFirebase::getAuthParams() {
    QJsonObject jsonObj;

    if (this->m_email == "") {
        return jsonObj;
    }

    jsonObj["email"]        = this->m_email;
    jsonObj["displayName"]  = this->m_name;

    jsonObj["localId"]      = this->_localId;
    jsonObj["idToken"]      = this->_accessToken;
    jsonObj["refreshToken"] = this->_refreshToken;
    jsonObj["expiresIn"]    = this->_expiresIn;

    return jsonObj;
}

void QFirebase::signin(QString email, QString password) {
    this->_authMethod(email, password, "relyingparty/verifyPassword");
}

void QFirebase::signup(QString email, QString password, QString name) {
    this->_authMethod(email, password, "relyingparty/signupNewUser", name);
}

void QFirebase::signinByAuthParams(QJsonObject auth_params) {
    if (!auth_params.contains("email")) {return;}
    if (!auth_params.contains("refreshToken")) {return;}

    this->_possibleEmail = auth_params["email"].toString();
    this->_possibleName = auth_params["displayName"].toString();

    this->_refresh(auth_params["refreshToken"].toString());
}

void QFirebase::signout() {
    this->_localId       = "";
    this->_accessToken   = "";
    this->_refreshToken  = "";
    this->_expiresIn     = -1;

    this->_setEmail("");
    this->_setName("");
}

void QFirebase::_refresh(QString refresh_token) {
    if (refresh_token.length() == 0) {qFatal("Token is empty");}

    QJsonObject jsonObj;

    jsonObj["grant_type"] = "refresh_token";
    jsonObj["refresh_token"] = refresh_token;

    QJsonDocument uploadDoc(jsonObj);

    Firebase *fb = new Firebase(REFRESH_URI, "token");
    fb->setValue(uploadDoc, "POST", "key=" + API_KEY);

    connect(fb, SIGNAL(eventResponseReady(QByteArray)), this, SLOT(_onAuthResponse(QByteArray)));
}

void QFirebase::_authMethod(QString email, QString password, QString methodName, QString displayName) {
    if (email.length() == 0) {qFatal("Trying to signin with empty password");}
    if (password.length() == 0) {qFatal("Trying to signin with empty password");}

    QJsonObject jsonObj;

    if (displayName != "") {
        jsonObj["displayName"] = displayName;
    }

    jsonObj["email"] = email;
    jsonObj["password"] = password;
    jsonObj["returnSecureToken"] = true;

    QJsonDocument uploadDoc(jsonObj);

    Firebase *fb = new Firebase(AUTH_URI, methodName);
    fb->setValue(uploadDoc, "POST", "key=" + API_KEY);

    connect(fb, SIGNAL(eventResponseReady(QByteArray)), this, SLOT(_onAuthResponse(QByteArray)));
}

void QFirebase::_setEmail(const QString &email) {
    if (email == m_email)
        return;

    m_email = email;
    emit emailChanged();
}

void QFirebase::_setName(const QString &name) {
    if (name == m_name)
        return;

    m_name = name;
    emit nameChanged();
}

void QFirebase::_onAuthResponse(QByteArray response) {
    qDebug() << "onAuthResponse";

    bool isRefresh = false;

    QJsonDocument document = QJsonDocument::fromJson(response);
    QJsonObject obj = document.object();

    qDebug() << obj;

    if (obj.contains("error")) {
        emit result(QFirebase::RESSTAT_FAIL, obj);

        this->_possibleEmail = "";
        return;
    }

    if (obj.contains("kind")) {
        isRefresh = false;
    } else {
        isRefresh = true;
    }

    this->_localId       = isRefresh ? obj["user_id"].toString()        : obj["localId"].toString();
    this->_accessToken   = isRefresh ? obj["id_token"].toString()       : obj["idToken"].toString();
    this->_refreshToken  = isRefresh ? obj["refresh_token"].toString()  : obj["refreshToken"].toString();
    this->_expiresIn     = isRefresh ? obj["expires_in"].toInt()        : obj["expiresIn"].toInt();

    QString new_email = this->_possibleEmail == ""  ? obj["email"].toString() : this->_possibleEmail;
    QString new_name  = this->_possibleName == ""   ? obj["displayName"].toString() : this->_possibleName;

    this->_setEmail(new_email);
    this->_setName(new_name);

    this->_possibleEmail = "";

    emit result(QFirebase::RESSTAT_SUCCESS, QJsonObject());
}
