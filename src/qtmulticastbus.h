/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtmulticastbus.h
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

#ifndef QTMULTICASTBUS_H
#define QTMULTICASTBUS_H

#include "qtupnpprotocol.h"
#include "qthttpheader.h"

#include <QObject>
#include <QNetworkInterface>
#include <QHostAddress>

namespace QUPnP
{
class QtUdpSocket;

/* this class is used to send/receive multicast messages via UDP.
  * note that this class could be also used to send/receive unicast messages.
  * when message received, it emits messageReceived() signal.
*/
class QtMulticastBus : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool active READ active)
    Q_PROPERTY(QHostAddress hostAddress READ hostAddress)
    Q_PROPERTY(QNetworkInterface networkInterface READ networkInterface)
    Q_PROPERTY(QString serverId READ serverId WRITE setServerId NOTIFY serverIdChanged)

public:
    static QtMulticastBus* createMulticastBus(const QNetworkInterface & interface = QNetworkInterface(), QObject *parent = 0);

    const QString& serverId() const {
        return m_serverId;
    }
    void setServerId(const QString& serverId);

    const QNetworkInterface& networkInterface() const {
        return m_networkInterface;
    }
    const QHostAddress& hostAddress() const {
        return m_hostAddress;
    }
    bool active() const {
        return m_active;
    }

    qint64 sendMessage(const char * data, qint64 size, const QHostAddress &address, quint16 port);
    qint64 sendMessage(const QByteArray & message, const QHostAddress &address, quint16 port);
    qint64 sendMessage(const QString & message, const QHostAddress &address, quint16 port);

Q_SIGNALS:
    void messageReceived(const QHostAddress sender, const quint16 senderPort, const QByteArray datagram);

    /* signals for properties change */
    void serverIdChanged(const QString serverId);

private Q_SLOTS:
    void requestSocketReadyRead();
    void multicastSocketReadyRead();

private:
    QtMulticastBus(const QNetworkInterface & interface, QObject *parent);

    bool init();

    QString makeServerId();
    bool initNetworkInfo();

    void readPendingDatagrams(QtUdpSocket *socket);

    QString m_serverId;
    QNetworkInterface m_networkInterface;
    QHostAddress m_hostAddress;
    bool m_active;

    QtUdpSocket *m_requestSocket;
    QtUdpSocket *m_multicastSocket;
};
}

#endif // QTMULTICASTBUS_H
