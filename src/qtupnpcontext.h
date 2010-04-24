/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtupnpcontext.h
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

#ifndef QTUPNPCONTEXT_H
#define QTUPNPCONTEXT_H

#include <QObject>

class QNetworkAccessManager;

namespace QUPnP
{
class QtMulticastBus;
class QtHttpServer;

class QtUpnpContext : public QObject
{
public:
    QtUpnpContext(QObject *parent = 0);
    QtUpnpContext(QtMulticastBus *ssdpBus, QtMulticastBus *genaBus, QtHttpServer *httpServer, QNetworkAccessManager *httpClient, QObject *parent = 0);

    QtMulticastBus *ssdpBus() const { return m_ssdpBus; }
    void setSsdpBus(QtMulticastBus *ssdpBus) { m_ssdpBus = ssdpBus; }

    QtMulticastBus *genaBus() const { return m_genaBus; }
    void setGenaBus(QtMulticastBus *genaBus) { m_genaBus= genaBus; }

    QtHttpServer *httpServer() const { return m_httpServer; }
    void setHttpServer(QtHttpServer *httpServer) { m_httpServer = httpServer; }

    QNetworkAccessManager *httpClient() const { return m_httpClient; }
    void setHttpClient(QNetworkAccessManager *httpClient) { m_httpClient = httpClient; }

private:
    QtMulticastBus *m_ssdpBus;
    QtMulticastBus *m_genaBus;
    QtHttpServer *m_httpServer;
    QNetworkAccessManager *m_httpClient;
};
}

#endif // QTUPNPCONTEXT_H
