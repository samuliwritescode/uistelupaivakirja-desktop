#include "serverinterface.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>
#include <QFile>
#include <QSettings>
#include <QMutexLocker>
#include "dblayer.h"
#include "trip.h"
#include "simpletrollingobjectfactory.h"

ServerInterface::ServerInterface(QObject *parent) :
    QObject(parent)
{    
    connect(this, SIGNAL(consume()), this, SLOT(run()), Qt::QueuedConnection);
    QSettings settings;
    m_serverAddr = settings.value("ServiceAddress").toString();
    m_username = settings.value("ServiceUsername").toString();
    m_password = settings.value("ServicePassword").toString();;

    QString path = settings.value("ProgramFolder").toString();
    m_serverPath = path+"/server/";
    m_reply = NULL;
}

void ServerInterface::commit()
{
    QMutexLocker lock(&m_mutex);
    m_requests.enqueue(SLOT(sendXML()));
    emit consume();
}

void ServerInterface::checkout()
{
    QMutexLocker lock(&m_mutex);
    m_requests.enqueue(SLOT(getXML()));
    emit consume();
}

void ServerInterface::run()
{
    qDebug() << "run";
    QMutexLocker lock(&m_mutex);

    if(m_requests.size() > 0)
    {
       if(m_reply == NULL)
       {
           login(m_requests.dequeue());
       }
    }
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
    m_reply = manager.get(req);
    connect(m_reply, SIGNAL(finished()), this, slot);
}

bool ServerInterface::checkError()
{
    disconnect(m_reply, SIGNAL(finished()));
    if(m_reply->error() == 0)
    {
        return true;
    }
    else
    {
        m_reply->deleteLater();
        qDebug() << "network replied with an error" << m_reply->errorString();
        qDebug() << m_reply->readAll();
        emit error("network replied with error");
        m_reply = NULL;
        emit consume();
        return false;
    }
}

void ServerInterface::getXML()
{
    m_reply->deleteLater();
    if(checkError())
    {
        QString doctype = m_getDoc.first();
        QNetworkRequest req(QUrl(m_serverAddr+doctype+"s"));
        m_reply = manager.get(req);
        connect(m_reply, SIGNAL(finished()), this, SLOT(getXMLDone()));
    }
}

void ServerInterface::getXMLDone()
{
    disconnect(m_reply, SIGNAL(finished()), this, SLOT(getXMLDone()));
    m_reply->deleteLater();
    if(checkError())
    {
        QString doctype = m_getDoc.first();
        m_getDoc.removeFirst();
        QFile saveTo(m_serverPath+doctype+".xml");
        saveTo.open(QIODevice::WriteOnly);
        saveTo.write(m_reply->readAll());
        saveTo.close();
        qDebug() << "got doc" << doctype;
        if(m_getDoc.isEmpty())
        {
            m_reply = NULL;
            emit consume();
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
    m_reply->deleteLater();
    if(checkError())
    {
        QSettings settings;
        QString path = settings.value("ProgramFolder").toString();
        QString doctype = m_getDoc.first();
        QNetworkRequest req(QUrl(m_serverAddr+doctype+"s"));
        QFile* xml = new QFile(path+"/database/"+doctype+".xml");
        xml->open(QIODevice::ReadOnly);
        m_reply = manager.post(req, xml);
        connect(m_reply, SIGNAL(finished()), this, SLOT(sentXMLDone()));
    }
}

void ServerInterface::sentXMLDone()
{
    disconnect(m_reply, SIGNAL(finished()), this, SLOT(sentXMLDone()));
    m_reply->deleteLater();
    if(checkError())
    {
        QString doctype = m_getDoc.first();
        m_getDoc.removeFirst();
        qDebug() << doctype << "sent: " << m_reply->readAll();
        if(m_getDoc.isEmpty())
        {
            m_reply = NULL;
            emit consume();
            emit commitDone();
        }
        else
        {
            sendXML();
        }
    }

}
