#include "serverinterface.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QSettings>
#include <QMutexLocker>
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
    m_reply = NULL;
}

bool ServerInterface::commit(QStringList docs, QList<QByteArray> data)
{
    if(m_reply == NULL)
    {
        m_getDoc = docs;
        m_getData = data;
        login(SLOT(sendXML()));
        return true;
    }
    else
    {
        return false;
    }
}

bool ServerInterface::checkout(QStringList docs)
{
    if(m_reply == NULL)
    {
        m_getDoc = docs;
        login(SLOT(getXML()));
        return true;
    }else
    {
        return false;
    }
}

void ServerInterface::login(const char* slot)
{
    QString connectionstring = m_serverAddr;
    connectionstring += "/login?";
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
        QString errorstr = m_reply->readAll();
        qDebug() << "content: " << errorstr;
        m_reply = NULL;
        emit error(errorstr);
        return false;
    }
}

void ServerInterface::getXML()
{
    m_reply->deleteLater();
    if(checkError())
    {
        QString doctype = m_getDoc.first();
        QNetworkRequest req(QUrl(m_serverAddr+"/"+doctype+"s"));
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
        QString doctype = m_getDoc.first();
        QNetworkRequest req(QUrl(m_serverAddr+"/"+doctype+"s"));
        req.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml");
        m_reply = manager.put(req, m_getData.first());
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
        m_getData.removeFirst();
        QByteArray response = m_reply->readAll();


        QDomDocument doc;

        if(doc.setContent(response))
        {
            QDomNodeList list = doc.elementsByTagName("TransactionTicket");
            if(list.count() == 1 &&
               list.at(0).isElement() &&
               list.at(0).hasChildNodes())
            {
                QDomElement element = list.at(0).toElement();
                QString text = element.childNodes().at(0).toText().nodeValue();
                emit commitFile(doctype, text.toInt());
            }
        }

        if(m_getDoc.isEmpty())
        {
            m_reply = NULL;
            emit commitDone();
        }
        else
        {
            sendXML();
        }
    }

}
