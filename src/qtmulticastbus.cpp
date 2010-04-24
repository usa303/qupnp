/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtmulticastbus.cpp
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

#include "qtmulticastbus.h"
#include "qtudpsocket.h"
#include "qthttpheader.h"

using namespace QUPnP;

QtMulticastBus::QtMulticastBus(const QNetworkInterface& interface, QObject *parent)
    : QObject(parent)
    , m_active(true)
    , m_serverId(makeServerId())
    , m_networkInterface(interface)
{

}

QtMulticastBus* QtMulticastBus::createSsdpClient(const QNetworkInterface & interface, QObject *parent)
{
    QtMulticastBus* client = new QtMulticastBus(interface, parent);
    if(!(client->init())) {
        qWarning() << "Can not create ssdp client.";
        delete client;
        client = NULL;
    }

    return client;
}

bool QtMulticastBus::init()
{
    /* Make sure all network info is available to us */
    if(!initNetworkInfo())
        return false;

    /* Set up sockets */
    m_requestSocket = QtUdpSocket::createUdpSocket(QtUdpSocket::Request, m_hostAddress, this);
    if(m_requestSocket) {
        connect(m_requestSocket, SIGNAL(readyRead()), this, SLOT(requestSocketReadyRead()));
    } else {
        qWarning() << "request socket init failed.";
        return false;
    }

    m_multicastSocket = QtUdpSocket::createUdpSocket(QtUdpSocket::Multicast, m_hostAddress, this);
    if(m_multicastSocket) {
        connect(m_multicastSocket, SIGNAL(readyRead()), this, SLOT(multicastSocketReadyRead()));
    } else {
        qWarning() << "multicast socket init failed.";
        return false;
    }

    return true;
}

void QtMulticastBus::setServerId(const QString& serverId)
{
    m_serverId = serverId;
}

qint64 QtMulticastBus::sendMessage(const char * data, qint64 size, const QHostAddress &address, quint16 port)
{
    if(m_active)
        /* We don't send messages in passive mode */
        return m_requestSocket->writeDatagram(data, size, address, port);
}

qint64 QtMulticastBus::sendMessage(const QByteArray & message, const QHostAddress &address, quint16 port)
{
    if(m_active)
        /* We don't send messages in passive mode */
        return m_requestSocket->writeDatagram(message, address, port);
}

qint64 QtMulticastBus::sendMessage(const QString & message, const QHostAddress &address, quint16 port)
{
    if(m_active)
        /* We don't send messages in passive mode */
        return m_requestSocket->writeDatagram(message.toAscii(), address, port);
}

QString QtMulticastBus::makeServerId()
{
    // TODO: make ther server id
    return "QSSDP";
}

bool QtMulticastBus::initNetworkInfo()
{
    bool ret = true;

    if(!(m_networkInterface.isValid())) {
        // if the specified interface is not valid, it gets the first network interface
        m_networkInterface = QNetworkInterface::interfaceFromIndex(0);
    }

    if(!(m_networkInterface.isValid())) {
        qWarning() << "No default route?";
        ret = false;
    } else {
        // TODO: here I just assume that the first one is the eth0 instead of lo
        m_hostAddress = m_networkInterface.addressEntries().first().ip();
        if(m_hostAddress.isNull()) {
            qWarning() << "Failed to find IP of network interface" << m_networkInterface.name();
            ret = false;
        }
    }

    return ret;
}

void QtMulticastBus::requestSocketReadyRead()
{
    readPendingDatagrams(m_requestSocket);
}

void QtMulticastBus::multicastSocketReadyRead()
{
    readPendingDatagrams(m_multicastSocket);
}

void QtMulticastBus::readPendingDatagrams(QtUdpSocket *socket)
{
    while(socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        socket->readDatagram(datagram.data(), datagram.size(),
                             &sender, &senderPort);

        /* We need the following lines to make sure the right client received
         * the packet. We won't need to do this if there was any way to tell
         * Mr. Unix that we are only interested in receiving multicast packets
         * on this socket from a particular interface but AFAIK that is not
         * possible, at least not in a portable way.
         */
        qDebug() << "received message from:" << sender.toString();
        qDebug() << "host address is:" << m_hostAddress.toString();
        if(sender != m_hostAddress)
            break;

        /* emit raw message to give a chance to handle it by developers themsleves */
        emit messageReceived(sender, senderPort, datagram);
    }
}
