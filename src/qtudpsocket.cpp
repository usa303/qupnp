/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtudpsocket.cpp
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

#include "qtudpsocket.h"
#include "qtupnpprotocol.h"

#include <config.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

using namespace QUPnP;

QtUdpSocket::QtUdpSocket(QObject *parent)
    : QUdpSocket(parent)
{

}

bool QtUdpSocket::init(QtUdpSocketType type, const QHostAddress  &address)
{
    struct in_addr iface_addr;
    struct ip_mreq mreq;
    bool boolean = true;
    quint8 ttl = 4;
    int res;

    /* Enable broadcasting */
    res = setsockopt(socketDescriptor(),
                     SOL_SOCKET,
                     SO_BROADCAST,
                     &boolean,
                     sizeof(boolean));
    if(res == -1)
        return false;

    /* TTL */
    res = setsockopt(socketDescriptor(),
                     IPPROTO_IP,
                     IP_MULTICAST_TTL,
                     &ttl,
                     sizeof(ttl));
    if(res == -1)
        return false;

    res = inet_aton(address.toString().toAscii().data(), &iface_addr);
    if(res == 0)
        return false;

    /* Set up additional things according to the type of socket desired */
    if(type == Multicast) {
        /* Enable multicast loopback */
        res = setsockopt(socketDescriptor(),
                         IPPROTO_IP,
                         IP_MULTICAST_LOOP,
                         &boolean,
                         sizeof(boolean));
        if(res == -1)
            return false;

        /* Set the interface */
        res = setsockopt(socketDescriptor(),
                         IPPROTO_IP,
                         IP_MULTICAST_IF,
                         &iface_addr,
                         sizeof(struct in_addr));
        if(res == -1)
            return false;

        /* Subscribe to multicast channel */
        res = inet_aton(SSDP_ADDR, &(mreq.imr_multiaddr));
        if(res == 0)
            return false;

        memcpy(&(mreq.imr_interface),
               &iface_addr,
               sizeof(struct in_addr));

        res = setsockopt(socketDescriptor(),
                         IPPROTO_IP,
                         IP_ADD_MEMBERSHIP,
                         &mreq,
                         sizeof(mreq));
        if(res == -1)
            return false;

        /* Bind to SSDP multicast port and address */
        /* Allow multiple sockets to use the same PORT number */
        if(!(bind(QHostAddress(SSDP_ADDR), SSDP_PORT, QUdpSocket::ShareAddress)))
            return false;
    } else {
        /* Bind to selected host address, system allocates the port number*/
        if(!(bind(address, 0)))
            return false;
    }

    return true;
}

QtUdpSocket* QtUdpSocket::createUdpSocket(QtUdpSocket::QtUdpSocketType type, const QHostAddress  &address, QObject *parent)
{
    QtUdpSocket* socket = new QtUdpSocket(parent);
    if(!(socket->init(type, address))) {
        qWarning() << "Can not create ssdp socket.";
        delete socket;
        socket = NULL;
    }

    return socket;
}
