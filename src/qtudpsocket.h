/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtudpsocket.h
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

#ifndef QTUDPSOCKET_H
#define QTUDPSOCKET_H

#include <QUdpSocket>

namespace QUPnP
{
class QtUdpSocket : public QUdpSocket
{
public:
    enum QtUdpSocketType {
        Request,
        Multicast
    };

    static QtUdpSocket* createUdpSocket(QtUdpSocketType type, const QHostAddress  &address, QObject *parent = 0);

private:
    QtUdpSocket(QObject *parent);

    /* setup and bind ssdp socket according to specific type */
    bool init(QtUdpSocketType type, const QHostAddress  &address);
};
}

#endif // QTUDPSOCKET_H
