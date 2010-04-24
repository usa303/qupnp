/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtssdpresourcebrowser.cpp
*   This file is part of QUPnP.
*
*   This library is free software; you can redistribute it and/or
*   modify it under the terms of the GNU Lesser General Public
*   License as published by the Free Software Foundation; either
*   version 2.1 of the License, or (at your option) any later version.
*
*   This library is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
*   Lesser General Public License for more details.
*
*   You should have received a copy of the GNU Lesser General Public
*   License along with this library; if not, write to the Free Software
*   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
****************************************************************************************/

#include "qtssdpresourcebrowser.h"
#include "qtupnpprotocol.h"
#include "qthttpheader.h"

#include <QTimer>
#include <QCoreApplication>
#include <QDateTime>

#define MAX_DISCOVERY_MESSAGES 3
#define DISCOVERY_FREQUENCY    500 /* 500 ms */

using namespace QUPnP;

QtSsdpResourceBrowser::QtSsdpResourceBrowser(QtMulticastBus *multicastBus, const QString &target, QObject *parent)
    : QObject(parent)
    , m_mx(SSDP_DEFAULT_MX)
    , m_multicastBus(multicastBus)
    , m_target(target)
{
    m_signalMapper = new QSignalMapper(this);
    connect(m_signalMapper, SIGNAL(mapped(const QString &)),
            this, SLOT(resourceExpire(const QString &)));

    connect(m_multicastBus, SIGNAL(messageReceived(const QHostAddress, const quint16, const QByteArray)),
            this, SLOT(messageReceived(const QHostAddress, const quint16, const QByteArray)));
}

QtSsdpResourceBrowser::~QtSsdpResourceBrowser()
{
    stopDiscovery();

    clearCache();
}

void QtSsdpResourceBrowser::setTarget(const QString& target)
{
    if(!m_active)
        return;

    m_target = target;

    /* setup regexp for :ver */
    // e.g. urn:domain-name:service:serviceType:ver
    //ver specifies the highest supported version of the service
    QStringList list = target.split(QRegExp(":[0-9]+"));
    if(list.count() == 1)
        m_targetRexExp = QRexExp(target);
    else
        m_targetRexExp = QRexExp(list.at(0) + ":[0-9]+");
}

void QtSsdpResourceBrowser::setMx(quint16 mx)
{
    if(mx == m_mx)
        return;

    m_mx = mx;
}

void QtSsdpResourceBrowser::setActive(bool active)
{
    if(active == m_active)
        return;

    m_active = active;

    if(m_active) {
        startDiscovery();
    } else {
        stopDiscovery();

        clearCache();
    }
}

void QtSsdpResourceBrowser::messageReceived(const QHostAddress sender, const quint16 senderPort, const QByteArray message)
{
    if(!m_active)
        return;

    /* Parse message */
    QtHttpHeader header(message);
    if(!(header.isValid())) {
        qWarning() << "Unhandled message:" << message;
        return;
    }

    switch(header.type()) {
    case QtHttpHeader::Request:
        if(header.method() == SSDP_NOTIFY_METHOD) {
            announcementReceived(header);
        } else {
            qWarning() << "Unhandled method:" << header.method();
        }
        break;
    case QtHttpHeader::Response:
        if(header.statusCode() == HTTP_OK) {
            discoveryResponseReceived(header);
        } else {
            qWarning() << "Unhandled status code:" << header.statusCode();
        }
        break;
    default:
        break;
    }
}

void QtSsdpResourceBrowser::discoveryResponseReceived(const QtHttpHeader& message)
{
    if(!(message.hasKey("ST")))
        return; /* No target specified */

    if(!checkTargetCompat(message.value("ST")))
        return; /* Target doesn't match */

    resourceAvailable(message);
}

void QtSsdpResourceBrowser::announcementReceived(const QtHttpHeader& message)
{
    if(!(message.hasKey("NT")))
        return; /* No target specified */

    if(!checkTargetCompat(message.value("NT")))
        return; /* Target doesn't match */

    if(!(message.hasKey("NTS")))
        return; /* No announcement type specified */

    /* Check announcement type */
    QString nts = message.value("NTS");
    if(nts == SSDP_ALIVE_NTS)
        resourceAvailable(message);
    else if(nts == SSDP_BYEBYE_NTS)
        resourceUnavailable(message);
}

void QtSsdpResourceBrowser::startDiscovery()
{
    /* Send one now */
    sendDiscoveryRequest();

    /* And schedule the rest for later */
    m_discoveryNum = 1;
    if(!m_discoveryTimer) {
        m_discoveryTimer = new QTimer(this);
        connect(m_discoveryTimer, SIGNAL(timeout()), this, SLOT(discoveryTimeout()));
    }

    m_discoveryTimer->start(DISCOVERY_FREQUENCY);
}

void QtSsdpResourceBrowser::stopDiscovery()
{
    if(m_discoveryTimer) {
        m_discoveryTimer->stop();
        m_discoveryNum = 0;
    }
}

void QtSsdpResourceBrowser::sendDiscoveryRequest()
{
    QString message = QString(SSDP_DISCOVERY_REQUEST).arg(m_target).arg.(m_mx).arg(QCoreApplication::applicationName());

    m_multicastBus->sendMessage(message, SSDP_ADDR, SSDP_PORT);
}

void QtSsdpResourceBrowser::discoveryTimeout()
{
    sendDiscoveryRequest();

    m_discoveryNum += 1;

    if(m_discoveryNum >= MAX_DISCOVERY_MESSAGES) {
        m_discoveryTimer->stop();
        m_discoveryNum = 0;
    }
}

bool QtSsdpResourceBrowser::checkTargetCompat(const QString &st)
{
    return ((m_target == QSSDP_ALL_RESOURCES) || m_targetRegExp.exactMatch(st));
}

void QtSsdpResourceBrowser::resourceAvailable(const QtHttpHeader &message)
{
    bool was_cached;

    if(!(message.hasKey("USN")))
        return; /* No USN specified */

    QString usn = message.value("USN");
    /* Get from cache, if possible */
    Resource *resource;
    if(m_resources.contains(usn)) {
        resource = m_resources[usn];
        /* Remove old timeout */
        resource->timer->stop();

        was_cached = true;
    } else {
        /* Create new Resource data structure */
        resource = new Resource;
        resource->usn = usn;
        resource->timer = new QTimer;
        connect(resource->timer, SIGNAL(timeout()), m_signalMapper, SLOT(map()));
        m_signalMapper->setMapping(resource->timer, resource->usn);

        m_resources.insert(usn, resource);

        was_cached = false;
    }

    /* Calculate new timeout */
    int timeout;
    QString header;
    if(message.hasKey("Cache-Control")) {
        header = message.value("Cache-Control");

        bool ok;

        /* find sub=string starting with "max-age = " */
        timeout = header.split('=', QString::SkipEmptyParts).last().toUInt(&ok);

        if(!ok) {
            qWarning() << QString("Invalid 'Cache-Control' header. Assuming "
                                  "default max-age of %1.\n"
                                  "Header was:\n%2").arg(SSDP_DEFAULT_MAX_AGE).arg(header);

            timeout = SSDP_DEFAULT_MAX_AGE;
        }
    } else {
        if(message.hasKey("Expires")) {
            QString expires = message.value("Expires");

            QDateTime expireTime = QDateTime::fromString(expires);
            QDateTime currentTime = QDateTime::currentDateTime();

            if(expireTime > currentTime)
                timeout = currentTime.secsTo(expireTime);
            else {
                qWarning() << QString("Invalid 'Expires' header. Assuming "
                                      "default max-age of %1.\n"
                                      "Header was:\n%2").arg(SSDP_DEFAULT_MAX_AGE).arg(expires);

                timeout = SSDP_DEFAULT_MAX_AGE;
            }
        } else {
            qWarning() << QString("No 'Cache-Control' nor any 'Expires' "
                                  "header was specified. Assuming default "
                                  "max-age of %1.").arg(SSDP_DEFAULT_MAX_AGE);

            timeout = SSDP_DEFAULT_MAX_AGE;
        }
    }

    resource->timer->start(timeout);

    /* Only continue with signal emission if this resource was not
     * cached already */
    if(was_cached)
        return;

    /* Build list of locations */
    QStringList locations;

    if(message.hasKey("Location"))
        locations.append(message.value("Location"));

    if(message.hasKey("AL")) {
        /* Parse AL header. The format is:
         * <uri1><uri2>... */
        /*extract each uri into locations */
        locations.append(message.value("AL").split(QRegExp("<*>")));
    }

    /* Emit signal */
    emit resourceAvailable(usn, locations);
}

void QtSsdpResourceBrowser::resourceUnavailable(const QtHttpHeader &message)
{
    if(!(message.hasKey("USN")))
        return; /* No USN specified */

    QString usn = message.value("USN");
    /* Only process if we were cached */
    if(!(m_resources.contains(usn)))
        return;

    emit resourceUnavailable(usn);

    delete m_resources[usn];
    m_resources.remove(usn);
}

void QtSsdpResourceBrowser::resourceExpire(Resource *resource)
{
    emit resourceUnavailable(resource->usn);

    m_resources.remove(resource->usn);
    delete resource;
}

void QtSsdpResourceBrowser::resourceExpire(const QString &usn)
{
    resourceExpire(m_resources[usn]);
}

void QtSsdpResourceBrowser::clearCache()
{
    foreach(Resource * resource, m_resources) {
        emit resourceUnavailable(resource->usn);
        delete resource;
    }

    m_resources.clear();
}
