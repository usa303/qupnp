/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtssdpresourcegroup.cpp
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

#include "qtssdpresourcegroup.h"
#include "qtupnpprotocol.h"
#include "qthttpheader.h"
#include "qtssdpresourcebrowser.h"

#include <QTimer>
#include <QDateTime>

#define DEFAULT_MESSAGE_DELAY 20

using namespace QUPnP;

QtSsdpResourceGroup::QtSsdpResourceGroup(QtMulticastBus *multicastBus, QObject *parent)
    : QObject(parent)
    , m_multicastBus(multicastBus)
    , m_maxAge(SSDP_DEFAULT_MAX_AGE)
    , m_messageDelay(DEFAULT_MESSAGE_DELAY)
{
    m_signalMapper = new QSignalMapper(this);
    connect(m_signalMapper, SIGNAL(mapped(int)),
            this, SLOT(discoveryResponseTimeout(int)));

    connect(m_multicastBus, SIGNAL(messageReceived(const QHostAddress, const quint16, const QByteArray)),
            this, SLOT(messageReceived(const QHostAddress, const quint16, const QByteArray)));

    m_messageDelayTimer = new QTimer(this);
    connect(m_messageDelayTimer, SIGNAL(timeout()), this, SLOT(processQueue()));
}

QtSsdpResourceGroup::~QtSsdpResourceGroup()
{
    foreach(Resource * resource, m_resources)
    resourceFree(resource);

    while(!(m_messageQueue.isEmpty())) {
        /* send messages without usual delay */
        if(m_available)
            processQueue();
    }
}

void QtSsdpResourceGroup::setMessageDelay(uint messageDelay)
{
    if(messageDelay == m_messageDelay)
        return;

    m_messageDelay = messageDelay;
}

void QtSsdpResourceGroup::setMaxAge(uint maxAge)
{
    if(maxAge == m_maxAge)
        return;

    m_maxAge = maxAge;
}

void QtSsdpResourceGroup::setAvailable(bool available)
{
    if(available == m_available)
        return;

    m_available = available;

    if(m_available) {
        /* We want to re-announce before actually timing out */
        int timeout = m_maxAge;
        if(timeout > 2)
            timeout = timeout / 2 - 1;

        /* Add re-announcement timer */
        if(!m_AnnouncementTimer) {
            m_AnnouncementTimer = new QTimer(this);
            connect(m_AnnouncementTimer, SIGNAL(timeout()), this, SLOT(announcementTimeout()));
        }
        m_AnnouncementTimer->start(timeout);

        /* Announce all resources */
        foreach(Resource * resource, m_resources)
        resourceAlive(resource);
    } else {
        /* Unannounce all resources */
        foreach(Resource * resource, m_resources)
        resourceByebye(resource);

        /* Remove re-announcement timer */
        m_AnnouncementTimer->stop();
    }
}

uint QtSsdpResourceGroup::addResource(const QString &target, const QString &usn, QStringList locations)
{
    Resource *resource = new Resource;
    resource->target = target;
    resource->usn = usn;
    resource->targetRegExp = createTargetRegExp(target);
    if(!(resource->targetRegExp.isValid())) {
        qWarning() << QString("Error compiling regular expression for '%1': %2").arg(target);

        resourceFree(resource);
        return 0;
    }

    resource->initialAliveSent = false;
    resource->locations.append(locations);

    m_resources.prepend(resource);

    resource->id = ++m_lastResourceId;

    if(m_available)
        resourceAlive(resource);

    return resource->id;
}

void QtSsdpResourceGroup::removeResource(uint resourceId)
{
    foreach(Resource * resource, m_resources) {
        if(resource->id == resourceId) {
            m_resources.removeAll(resource);
            resourceFree(resource);

            return;
        }
    }
}

void QtSsdpResourceGroup::announcementTimeout()
{
    /* Re-announce all resources */
    foreach(Resource * resource, m_resources) {
        resourceAlive(resource);
    }
}

void QtSsdpResourceGroup::messageReceived(const QHostAddress sender, const quint16 senderPort, const QByteArray message)
{
    /* Only process if we are available */
    if(!m_available)
        return;

    /* Parse message */
    QtHttpHeader header(message);
    if(!(header.isValid())) {
        qWarning() << "Unhandled message:" << message;
        return;
    }

    switch(header.type()) {
    case QtHttpHeader::Request:
        /* We only handle discovery requests */
        if(header.method() == SSDP_SEARCH_METHOD) {
            discoveryRequestReceived(header);
        } else {
            qWarning() << "Unhandled method:" << header.method();
        }
        break;
    case QtHttpHeader::Response:
    default:
        break;
    }
}

void QtSsdpResourceGroup::discoveryRequestReceived(const QtHttpHeader &header)
{
    /* Extract target */
    if(!(header.hasKey("ST"))) {
        qWarning() << "Discovery request did not have an ST header";

        return;
    }
    QString target = header.value("ST");

    /* Is this the "ssdp:all" target? */
    bool wantAll;
    wantAll = (target == QSSDP_ALL_RESOURCES);

    /* Extract MX */
    int mx;
    if(!(header.hasKey("MX")))
        mx = header.value("MX").toInt();
    else
        mx = SSDP_DEFAULT_MX;

    /* Find matching resource */
    foreach(Resource * resource, m_resources) {
        if(wantAll || resource->targetRegExp.exactMatch(target)) {
            /* Match. */
            int timeout;

            /* Get a random timeout from the interval [0, mx] */
            while() {
                timeout = qrand();
                if(timeout <= mx * 1000)
                    break;
            }

            /* Prepare response */
            DiscoveryResponse *response = new DiscoveryResponse;

            response->destIp   = sender;
            response->destPort = senderPort;
            response->resource  = resource;
            response->id = ++m_discoveryResponseId;

            if(wantAll)
                response->target = resource->target;
            else
                response->target = target;

            /* Add timeout */
            response->timer = new QTimer;
            connect(response->timer, SIGNAL(timeout()), m_signalMapper, SLOT(map()));
            m_signalMapper->setMapping(response->timer, response->id);

            m_discoveryResponses.insert(response->id, response);

            response->timer->start(timeout);

            /* Add to resource */
            resource->responses.prepend(response);
        }
    }
}

QString QtSsdpResourceGroup::constructAl(Resource *resource)
{
    QString al;

    /* from the second one */
    if(resource->locations.size() > 1) {
        al.append("AL: ");
        for(int i = 1; i < resource->locations.size(); ++i) {
            al.append('<');
            al.append(resource->locations.at(i));
            al.append('>');
        }
    }

    return al;
}

void QtSsdpResourceGroup::discoveryResponseTimeout(int id)
{
    discoveryResponseTimeout(m_discoveryResponses[id]);
}

void QtSsdpResourceGroup::discoveryResponseTimeout(DiscoveryResponse *response)
{
    /* Send message */
    QString message = QString(SSDP_DISCOVERY_RESPONSE)
                      .arg(response->resource->locations.first())
                      .arg(constructAl(response->resource))
                      .arg(response->resource->usn)
                      .arg(m_multicastBus->serverId())
                      .arg(m_maxAge)
                      .arg(response->target)
                      .arg(QDateTime::currentDateTime().toString());

    m_multicastBus->sendMessage(message, response->destIp, response->destPort);

    discoveryResponseFree(response);
}

void QtSsdpResourceGroup::processQueue()
{
    if(m_messageQueue.isEmpty()) {
        /* this is the timeout after last message in queue */
        m_messageDelayTimer->stop();
    } else {
        QString message = m_messageQueue.dequeue();
        m_multicastBus->sendMessage(message, SSDP_ADDR, SSDP_PORT);
    }
}

void QtSsdpResourceGroup::queueMessage(const QString &message)
{
    m_messageQueue.enqueue(message);

    if(!(m_messageDelayTimer->isActive())) {
        /* nothing in the queue: process message immediately
           and add a timeout for (possible) next message */
        processQueue();
        m_messageDelayTimer->start(m_messageDelay);
    }
}

void QtSsdpResourceGroup::resourceAlive(Resource *resource)
{
    if(!resource->initialAliveSent) {
        /* Unannounce before first announce. This is done to
           minimize the possibility of control points thinking
           that this is just a reannouncement. */
        resourceByebye(resource);

        resource->initialAliveSent = true;
    }

    /* Send message */
    QString message = QString(SSDP_ALIVE_MESSAGE)
                      .arg(m_maxAge)
                      .arg(resource->locations.first())
                      .arg(constructAl(resource))
                      .arg(m_multicastBus->serverId())
                      .arg(resource->target)
                      .arg(resource->usn);

    queueMessage(message);
}

void QtSsdpResourceGroup::resourceByebye(Resource *resource)
{
    /* Queue message */
    QString message = QString(SSDP_BYEBYE_MESSAGE)
                      .arg(resource->target)
                      .arg(resource->usn);

    queueMessage(message);
}

void QtSsdpResourceGroup::resourceFree(Resource *resource)
{
    if(m_available)
        resourceByebye(resource);

    delete resource;
}

void QtSsdpResourceGroup::discoveryResponseFree(DiscoveryResponse *response)
{
    m_discoveryResponses.remove(response->id);
    delete response;
}

QRegExp QtSsdpResourceGroup::createTargetRegExp(const QString &target)
{
    QStringList list = target.split(QRegExp(":[0-9]+"));
    if(list.count() == 1)
        return QRexExp(target);
    else
        return QRexExp(list.at(0) + ":[0-9]+");
}
