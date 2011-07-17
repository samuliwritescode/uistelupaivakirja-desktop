#ifndef SERVERINTERFACE_H
#define SERVERINTERFACE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStringList>

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
    void error(const QString& message);

public slots:
    void sentXMLDone();
    void getXMLDone();
    void sendXML();
    void getXML();

private:
    void login(const char*);
    bool checkError();

private:
    QNetworkAccessManager manager;
    QNetworkReply* reply;
    QStringList m_getDoc;
    QString m_serverAddr;
    QString m_username;
    QString m_password;
    QString m_serverPath;
};

#endif // SERVERINTERFACE_H
