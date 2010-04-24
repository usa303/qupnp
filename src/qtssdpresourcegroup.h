/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtssdpresourcegroup.h
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

#ifndef QTSSDPRESOURCEGROUP_H
#define QTSSDPRESOURCEGROUP_H

#include "qtmulticastbus.h"

#include <QObject>
#include <QHash>
#include <QRegExp>
#include <QSignalMapper>
#include <QStringList>
#include <QHostAddress>
#include <QQueue>

class QTimer;

namespace QUPnP
{
class QtHttpHeader;

class QtSsdpResourceGroup : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool available READ available WRITE setAvailable NOTIFY availableChanged)
    Q_PROPERTY(QtMulticastBus* multicastBus READ multicastBus NOTIFY clientChanged)
    Q_PROPERTY(uint maxAge READ maxAge WRITE setMaxAge NOTIFY maxAgeChanged)
    Q_PROPERTY(uint messageDelay READ messageDelay WRITE setMessageDelay NOTIFY messageDelayChanged)

    struct DiscoveryResponse;
    struct Resource {
        ~Resource() {
            foreach(DiscoveryResponse * response, responses)
            delete response;
        }

        QRegExp targetRegExp;
        QString target;
        QString usn;
        QStringList locations;
        QList<DiscoveryResponse *> responses;
        uint id;
        bool initialAliveSent;
    };

    struct DiscoveryResponse {
        ~DiscoveryResponse() {
            delete timer;
            resource->responses.removeAll(this);
        }

        QHostAddress destIp;
        quint16   destPort;
        QString target;
        Resource *resource;

        QTimer *timer;
        uint id;
    };

public:
    QtSsdpResourceGroup(QtMulticastBus *multicastBus, QObject *parent = 0);
    ~QtSsdpResourceGroup();

    QtMulticastBus* multicastBus() const {
        return m_multicastBus;
    }

    void setMessageDelay(uint messageDelay);
    uint messageDelay() {
        return m_messageDelay;
    }

    void setMaxAge(uint maxAge);
    uint maxAge() const {
        return m_maxAge;
    }

    void setAvailable(bool available);
    bool available() const {
        return m_available;
    }

    uint addResource(const QString &target, const QString &usn, QStringList locations);
    void removeResource(uint resourceId);

Q_SIGNALS:
    /* signals for properties change */
    void availableChanged(bool available);
    void clientChanged(QtMulticastBus *multicastBus);
    void maxAgeChanged(uint maxAge);
    void messageDelayChanged(uint messageDelay);

private Q_SLOTS:
    void messageReceived(const QHostAddress sender, const quint16 senderPort, const QByteArray message);

    void discoveryResponseTimeout(int id);
    void announcementTimeout();

private:
    void discoveryRequestReceived(const QtHttpHeader &header);
    QString constructAl(Resource *resource);
    void discoveryResponseTimeout(DiscoveryResponse *response);
    void processQueue();
    void queueMessage(const QString &message);
    void resourceAlive(Resource *resource);
    void resourceByebye(Resource *resource);
    void resourceFree(Resource *resource);
    void discoveryResponseFree(DiscoveryResponse *response);
    QRegExp createTargetRegExp(const QString &target);

    QtMulticastBus *m_multicastBus;
    uint m_maxAge;
    bool m_available;
    QList<Resource *> m_resources;
    QHash<uint, DiscoveryResponse *> m_discoveryResponses;
    uint m_discoveryResponseId;
    QTimer *m_AnnouncementTimer;
    QTimer *m_messageDelayTimer;
    uint m_lastResourceId;
    uint m_messageDelay;
    QQueue<QString> m_messageQueue;
    QSignalMapper *m_signalMapper;
};
}

#endif // QTSSDPRESOURCEGROUP_H
