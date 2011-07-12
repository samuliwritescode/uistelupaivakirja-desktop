#include "serverinterface.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>
#include <QFile>
#include <QSettings>
#include "dblayer.h"
#include "trip.h"
#include "simpletrollingobjectfactory.h"

ServerInterface::ServerInterface(QObject *parent) :
    QObject(parent)
{
    m_serverAddr = "http://localhost:8080/uistelu/";
    m_username = "cape";
    m_password = "kek";
}

void ServerInterface::commit()
{
    sendXML("/Users/cape/uistelu/database/trip.xml");
}

void ServerInterface::checkout()
{
    login();
}

void ServerInterface::sendXML(const QString& file)
{
    QNetworkRequest req(QUrl(m_serverAddr+"trips"));
    QFile* xml = new QFile(file);
    xml->open(QIODevice::ReadOnly);
    reply = manager.post(req, xml);
    connect(reply, SIGNAL(finished()), this, SLOT(sentXMLDone()));
}

void ServerInterface::login()
{
    QString connectionstring = m_serverAddr;
    connectionstring += "login?";
    connectionstring += "j_username=";
    connectionstring += m_username;
    connectionstring += "&j_password=";
    connectionstring += m_password;
    QUrl connectionurl(connectionstring);
    QNetworkRequest req(connectionurl);
    reply = manager.get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(loginDone()));
}

void ServerInterface::loginDone()
{
    disconnect(reply, SIGNAL(finished()));
    if(reply->error() == 0)
    {
        getXML();
        //sendXML("/Users/cape/uistelu/database/trip.xml");
    }
    else
    {
        qCritical() << "Cannot login";
    }
}

void ServerInterface::getXML()
{
    QNetworkRequest req(QUrl(m_serverAddr+"trips"));
    reply = manager.get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(getXMLDone()));
}

void ServerInterface::getXMLDone()
{
    disconnect(reply, SIGNAL(finished()));
    if(reply->error() == 0)
    {
        QFile saveTo("/Users/cape/uistelu/server/trip.xml");
        saveTo.open(QIODevice::WriteOnly);
        saveTo.write(reply->readAll());
        saveTo.close();
        qDebug() << "trips got";
        emit checkoutDone("/Users/cape/uistelu/server/");
    }
    else
    {
        qDebug() << "trips cannot be get";
        qDebug() << "status: " << reply->error();
    }
}


void ServerInterface::sentXMLDone()
{
    disconnect(reply, SIGNAL(finished()));
    if(reply->error() == 0)
    {
        qDebug() << "trips sent: " << reply->readAll();
        emit commitDone();
    }
    else
    {
        qDebug() << "trips cannot be sent";
        qDebug() << "status: " << reply->error();
        qDebug() << "response: " << reply->readAll();
    }

}
