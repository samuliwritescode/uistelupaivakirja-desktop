#ifndef SERVERINTERFACE_H
#define SERVERINTERFACE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class ServerInterface : public QObject
{
    Q_OBJECT
public:
    explicit ServerInterface(QObject *parent = 0);
    void commit();
    void checkout();

signals:
    void checkoutDone(const QString& folder);
    void commitDone();

public slots:
    void loginDone();
    void sentXMLDone();
    void getXMLDone();

private:
    void login();
    void sendXML(const QString& file);
    void getXML();

private:
    QNetworkAccessManager manager;
    QNetworkReply* reply;
    QString m_serverAddr;
    QString m_username;
    QString m_password;
};

#endif // SERVERINTERFACE_H
