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
    QSettings settings;
    m_serverAddr = settings.value("ServiceAddress").toString();
    m_username = settings.value("ServiceUsername").toString();
    m_password = settings.value("ServicePassword").toString();;

    QString path = settings.value("ProgramFolder").toString();
    m_serverPath = path+"/server/";
    reply = NULL;
}

void ServerInterface::commit()
{
    if(reply == NULL)
        login(SLOT(sendXML()));
    else
        emit error("replies in progress");
}

void ServerInterface::checkout()
{
    if(reply == NULL)
        login(SLOT(getXML()));
    else
        emit error("replies in progress");
}

void ServerInterface::login(const char* slot)
{
    m_getDoc.clear();
    m_getDoc.append("trip");
    m_getDoc.append("place");
    m_getDoc.append("lure");

    QString connectionstring = m_serverAddr;
    connectionstring += "login?";
    connectionstring += "j_username=";
    connectionstring += m_username;
    connectionstring += "&j_password=";
    connectionstring += m_password;
    QUrl connectionurl(connectionstring);
    QNetworkRequest req(connectionurl);
    reply = manager.get(req);
    connect(reply, SIGNAL(finished()), this, slot);
}

bool ServerInterface::checkError()
{
    disconnect(reply, SIGNAL(finished()));
    if(reply->error() == 0)
    {
        return true;
    }
    else
    {
        reply->deleteLater();
        qDebug() << "network replied with an error" << reply->errorString();
        qDebug() << reply->readAll();
        emit error("network replied with error");
        reply = NULL;
        return false;
    }
}

void ServerInterface::getXML()
{
    reply->deleteLater();
    if(checkError())
    {
        QString doctype = m_getDoc.first();
        QNetworkRequest req(QUrl(m_serverAddr+doctype+"s"));
        reply = manager.get(req);
        connect(reply, SIGNAL(finished()), this, SLOT(getXMLDone()));
    }
}

void ServerInterface::getXMLDone()
{
    disconnect(reply, SIGNAL(finished()), this, SLOT(getXMLDone()));
    reply->deleteLater();
    if(checkError())
    {
        QString doctype = m_getDoc.first();
        m_getDoc.removeFirst();
        QFile saveTo(m_serverPath+doctype+".xml");
        saveTo.open(QIODevice::WriteOnly);
        saveTo.write(reply->readAll());
        saveTo.close();
        qDebug() << "got doc" << doctype;
        if(m_getDoc.isEmpty())
        {
            reply = NULL;
            emit checkoutDone(m_serverPath);
        }
        else
        {
            getXML();
        }
    }
}

void ServerInterface::sendXML()
{
    reply->deleteLater();
    if(checkError())
    {
        QSettings settings;
        QString path = settings.value("ProgramFolder").toString();
        QString doctype = m_getDoc.first();
        QNetworkRequest req(QUrl(m_serverAddr+doctype+"s"));
        QFile* xml = new QFile(path+"/database/"+doctype+".xml");
        xml->open(QIODevice::ReadOnly);
        reply = manager.post(req, xml);
        connect(reply, SIGNAL(finished()), this, SLOT(sentXMLDone()));
    }
}

void ServerInterface::sentXMLDone()
{
    disconnect(reply, SIGNAL(finished()), this, SLOT(sentXMLDone()));
    reply->deleteLater();
    if(checkError())
    {
        QString doctype = m_getDoc.first();
        m_getDoc.removeFirst();
        qDebug() << doctype << "sent: " << reply->readAll();
        if(m_getDoc.isEmpty())
        {
            reply = NULL;
            emit commitDone();
        }
        else
        {
            sendXML();
        }
    }

}
