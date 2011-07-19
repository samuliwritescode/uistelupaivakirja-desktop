#ifndef SERVERINTERFACE_H
#define SERVERINTERFACE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStringList>
#include <QQueue>
#include <QMutex>

class ServerInterface : public QObject
{
    Q_OBJECT
public:
    explicit ServerInterface(QObject *parent = 0);
    bool commit(QStringList, QList<QByteArray>);
    bool checkout(QStringList);

signals:
    void checkoutDone(const QString&);
    void commitFile(const QString&, int);
    void commitDone();
    void error(const QString&);

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
    QNetworkReply* m_reply;
    QStringList m_getDoc;
    QList<QByteArray> m_getData;
    QString m_serverAddr;
    QString m_username;
    QString m_password;
    QString m_serverPath;
    QMutex m_mutex;
};

#endif // SERVERINTERFACE_H
