/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtssdpresourcebrowser.h
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

#ifndef QTSSDPRESOURCEBROWSER_H
#define QTSSDPRESOURCEBROWSER_H

#include "qtmulticastbus.h"

#include <QObject>
#include <QHash>
#include <QRegExp>
#include <QSignalMapper>
#include <QStringList>

/**
 * QSSDP_ALL_RESOURCES
 *
 * SSDP search target for finding all possible resources.
 **/
#define QSSDP_ALL_RESOURCES "ssdp:all"

class QTimer;
class QHostAddress;

namespace QUPnP
{
class QtHttpHeader;

class QtSsdpResourceBrowser : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(QtMulticastBus* multicastBus READ multicastBus NOTIFY clientChanged)
    Q_PROPERTY(quint16 mx READ mx WRITE setMx NOTIFY mxChanged)
    Q_PROPERTY(QString target READ target WRITE setTarget NOTIFY targetChanged)

    struct Resource {
        ~Resource() {
            delete timer;
        }
        QString usn;
        QTimer *timer;
    };

public:
    QtSsdpResourceBrowser(QtMulticastBus *multicastBus, const QString &target, QObject *parent = 0);
    ~QtSsdpResourceBrowser();

    QtMulticastBus* multicastBus() const {
        return m_multicastBus;
    }

    void setTarget(const QString& target);
    const QString target() {
        return m_target;
    }

    void setMx(quint16 mx);
    quint16 mx() const {
        return m_mx;
    }

    void setActive(bool active);
    bool active() const {
        return m_active;
    }

Q_SIGNALS:
    void resourceAvailable(const QString &usn, QStringList locations);
    void resourceUnavailable(const QString &usn);

    /* signals for properties change */
    void activeChanged(bool active);
    void clientChanged(QtMulticastBus *multicastBus);
    void mxChanged(quint16 mx);
    void targetChanged(const QString& target);

private Q_SLOTS:
    void messageReceived(const QHostAddress sender, const quint16 senderPort, const QByteArray message);

private:
    void discoveryResponseReceived(const QtHttpHeader& message);
    void announcementReceived(const QtHttpHeader& message);

    void startDiscovery();
    void stopDiscovery();

    void sendDiscoveryRequest();
    void discoveryTimeout();
    bool checkTargetCompat(const QString &st);

    void resourceAvailable(const QtHttpHeader &message);
    void resourceUnavailable(const QtHttpHeader &message);
    void resourceExpire(Resource *resource);
    void resourceExpire(const QString &usn);

    void clearCache();

    bool m_active;
    QtMulticastBus *m_multicastBus;
    quint16 m_mx;
    QString m_target;

    QHash<QString, Resource*> m_resources;
    quint32 m_discoveryNum;
    //quint64 m_messageReceivedId;
    QRegExp m_targetRegExp;
    QTimer *m_discoveryTimer;
    QSignalMapper *m_signalMapper;
};
}

#endif // QTSSDPRESOURCEBROWSER_H
