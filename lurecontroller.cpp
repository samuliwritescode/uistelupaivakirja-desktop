#include "lurecontroller.h"
#include "trollingexception.h"
#include "singletons.h"

LureController::LureController():
        m_lure(NULL),
        m_onlyFavorites(false)
{   
}

void LureController::buttonEvent(EUISource source)
{
    try
    {
        switch(source)
        {
        case eLureUndo:
            {
                int id = m_lure->getId();
                Singletons::model()->reset(m_lure);
                m_lure = Singletons::model()->getLure(id);
            } break;
        case eLureNew:
            m_lure = Singletons::model()->getLure();
            break;
        case eLureSave: Singletons::model()->commit(m_lure); break;
        case eLureDelete:
            if(showConfirmationMessage(tr("Poistetaanko viehe?")))
            {
                Singletons::model()->remove(m_lure);
                m_lure = NULL;
            }
            break;
        default: break;
        }
    }
    catch(TrollingException e)
    {
        showErrorMessage(e.toString());
    }
    sendNotificationToObservers(Controller::eLureUpdated);
    sendNotificationToObservers(Controller::eLureListUpdated);
}

void LureController::booleanEvent(EUISource source, bool value)
{
    switch(source)
    {
    case eLureSearchFavorites:
        m_onlyFavorites = value;
        sendNotificationToObservers(Controller::eLureListUpdated);
        return;
    default: break;
    }

    if(!m_lure) return;
    switch(source)
    {
    case eLureFavorite: m_lure->setFavorite(value); break;
    case eLureNotVisible: m_lure->setNotVisible(value); break;
    default: break;
    }
    sendNotificationToObservers(Controller::eLureInternalUpdate);
}

void LureController::textEvent(EUISource source, const QString& value)
{
    switch(source)
    {
    case eLureSearch:
        m_searchString = value;
        sendNotificationToObservers(Controller::eLureListUpdated);
        return;
    default: break;
    }

    if(!m_lure) return;

    switch(source)
    {
    case eLureType: m_lure->setLureType(value); break;
    case eLureMaker: m_lure->setMaker(value);break;
    case eLureModel: m_lure->setModel(value); break;
    case eLureSize: m_lure->setSize(value); break;
    case eLureColor: m_lure->setColor(value); break;
    case eLureColorBack: m_lure->setColorBack(value); break;
    case eLureColorSide: m_lure->setColorSide(value); break;
    case eLureColorBelly: m_lure->setColorBelly(value); break;
    case eLureColorClass: m_lure->setColorClass(value); break;
    case eLureColorContrast: m_lure->setColorContrast(value); break;
    case eLureNickName: m_lure->setNickName(value); break;
    default: break;
    }
    sendNotificationToObservers(Controller::eLureInternalUpdate);
}

void LureController::selectPlace(int value)
{
    if(m_lure && m_lure->isUnsaved())
    {
        int choice = showChoiseMessage(tr("Nykyinen viehe on tallentamatta. Haluatko tallentaa muutokset?"));
        if(choice == MessageDisplay::eCancel)
        {
            sendNotificationToObservers(Controller::eLureListUpdated);
            return;
        }
        else if(choice == MessageDisplay::eYes)
        {
            Singletons::model()->commit(m_lure);
        }
        else
        {
            Singletons::model()->reset(m_lure);
        }
        sendNotificationToObservers(Controller::eLureListUpdated);
    }
    m_lure = Singletons::model()->getLure(value);
}

void LureController::intEvent(EUISource source, int value)
{
    switch(source)
    {
    case eLureList: selectPlace(value); break;
    default: break;
    }
    sendNotificationToObservers(Controller::eLureUpdated);
}

int LureController::getIntValue(EUISource source)
{
    if(!m_lure) return 0;

    switch(source)
    {
    case eLureList: return m_lure->getId();
    default: break;
    }
    return 0;
}

bool LureController::getBooleanValue(EUISource source)
{
    if(!m_lure) return false;
    switch(source)
    {
    case eLureList: return true; break;
    case eUnsavedChanges: return m_lure->isUnsaved(); break;
    case eLureFavorite: return m_lure->getFavorite(); break;
    case eLureNotVisible: return m_lure->getNotVisible(); break;
    default: break;
    }
    return false;
}

QString LureController::getTextValue(EUISource source)
{
    if(!m_lure) return QString();

    switch(source)
    {
    case eLureMaker: return m_lure->getMaker();break;
    case eLureModel: return m_lure->getModel(); break;
    case eLureSize: return m_lure->getSize(); break;
    case eLureColor: return m_lure->getColor(); break;
    case eLureColorBack: return m_lure->getColorBack(); break;
    case eLureColorBelly: return m_lure->getColorBelly(); break;
    case eLureColorSide: return m_lure->getColorSide(); break;
    case eLureColorClass: return m_lure->getColorClass(); break;
    case eLureColorContrast: return m_lure->getColorContrast(); break;
    case eLureType: return m_lure->getLureType(); break;
    case eLureNickName: return m_lure->getNickName(); break;
    default: break;
    }
    return QString();
}

QList<QPair<QString, int> > LureController::getLureList()
{
    QList<QPair<QString, int> > retval;
    QMap<int, Lure*> lurelist = Singletons::model()->getLures();

    for(QMap<int, Lure*>::iterator iter = lurelist.begin(); iter != lurelist.end(); iter++)
    {
        Lure* lure = iter.value();
        if(lure->getNotVisible())
            continue;

        QPair<QString, int> pair;
        pair.first = lureName(lure);
        pair.second = lure->getId();

        if(match(lure))
        {
            retval.push_back(pair);
        }
    }
    qSort(retval);
    return retval;
}

bool LureController::match(Lure* lure)
{
    if(m_searchString.isEmpty() && !m_onlyFavorites)
        return true;

    QString lurename = lureName(lure);
    bool bMatch = true;

    if(!m_searchString.isEmpty() &&
       !lurename.toLower().contains(m_searchString.toLower()))
    {
        bMatch = false;
    }

    if(m_onlyFavorites && !lure->getFavorite())
    {
        bMatch = false;
    }

    return bMatch;
}

QString LureController::lureName(Lure* lure)
{
    if(lure)
    {
        return lure->getMaker()+" "+lure->getModel()+" "+lure->getSize()+" "+lure->getColor();
    }
    else
    {
        return QString();
    }
}

QList<QList<QString> > LureController::getLureListLong()
{
    QList<QList<QString> > retval;
    QMap<int, Lure*> lurelist = Singletons::model()->getLures();

    for(QMap<int, Lure*>::iterator iter = lurelist.begin(); iter != lurelist.end(); iter++)
    {
        Lure* lure = iter.value();
        QList<QString> lurehash;
        lurehash.push_back(lure->getMaker());
        lurehash.push_back(lure->getModel());
        lurehash.push_back(lure->getSize());
        lurehash.push_back(lure->getColor());
        lurehash.push_back(lure->getColorBack());
        lurehash.push_back(lure->getColorSide());
        lurehash.push_back(lure->getColorBelly());
        lurehash.push_back(lure->getColorContrast());
        lurehash.push_back(lure->getColorClass());
        lurehash.push_back(lure->getLureType());
        lurehash.push_back(lure->getNickName());
        if(lure->getFavorite())
            lurehash.push_back(tr("suosikki"));
        else
            lurehash.push_back(tr(""));

        if(lure->getNotVisible())
            lurehash.push_back(tr("ei näy"));
        else
            lurehash.push_back(tr(""));

        lurehash.push_back(QString::number(lure->getId()));
        retval.push_back(lurehash);
    }
    return retval;
}

QStringList LureController::getLureColumns()
{
    QStringList retval;
    retval << tr("Valmistaja") <<
           tr("Malli") <<
           tr("Koko") <<
           tr("Väri") <<
           tr("Selkä") <<
           tr("Kylki") <<
           tr("Vatsa") <<
           tr("Kontrasti") <<
           tr("Luokka") <<
           tr("Tyyppi") <<
           tr("Lempinimi") <<
           tr("Suosikki") <<
           tr("Näkyy");
    return retval;
}

QStringList LureController::getAlternatives(EUISource source)
{
    QStringList retval;

    QMap<int, Lure*> lures = Singletons::model()->getLures();
    foreach(Lure* lure, lures)
    {
        QString value;
        switch(source)
        {
            case eLureMaker: value = lure->getMaker(); break;
            case eLureModel: value = lure->getModel(); break;
            case eLureColor: value = lure->getColor(); break;
            case eLureColorBack: value = lure->getColorBack(); break;
            case eLureColorBelly: value = lure->getColorBelly(); break;
            case eLureColorSide: value = lure->getColorSide(); break;
            //case eLureColorClass: value = lure->getColorClass(); break;
            case eLureColorContrast: value = lure->getColorContrast(); break;
            case eLureSize: value = lure->getSize(); break;
            case eLureType: value = lure->getLureType(); break;
            default: break;
        }

        if(!retval.contains(value))
        {
            retval.push_back(value);
        }
    }

    retval.sort();
    return retval;
}
