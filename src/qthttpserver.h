/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qthttpserver.h
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

#ifndef QTHTTPSERVER_H
#define QTHTTPSERVER_H

#include <QTcpServer>
#include <QUrl>
#include <QAuthenticator>
#include <QHash>
#include <QSet>

namespace QUPnP
{
class QtHttpServerHandlerInterface;
class QtHttpHeader;

class QtHttpServer : public QTcpServer
{
    Q_OBJECT

public:
    enum HttpServerMode {
        Http,
        Https
    };

    QtHttpServer(HttpServerMode mode = Http, QObject *parent = 0);
    ~QtHttpServer();

    void pause();
    void resume();

    HttpServerMode mode() const {
        return m_mode;
    }
    void setMode(HttpServerMode mode);

    QString serverHeader() const {
        return m_serverHeader;
    }
    void setServerHeader(const QString &header);

    void addHandler(const QUrl &url, QtHttpServerHandlerInterface *handler);
    void removeHandler(const QUrl &url);

    void addAuthenticator(const QAuthenticator &authenticator);
    void removeAuthenticator(const QAuthenticator &authenticator);

    void hostFile(const QUrl &serverUrl, const QString &localFile);
    void unhostFile(const QUrl &serverUrl);

protected:
    void incomingConnection(int socketDescriptor);

private:
    void readClient();
    bool isAuthenticated(const QtHttpHeader &header);
    void defaultHandling(const QtHttpHeader &header);
    void authenticationFailedHandling(const QtHttpHeader &header);
    bool isKeepAlive(const QtHttpHeader &requestHeader, const QtHttpHeader &responseHeader);

    HttpServerMode m_mode;
    QString m_serverHeader;
    bool m_disabled;
    QHash<QUrl, QtHttpServerHandlerInterface *> m_handlers;
    QSet<QAuthenticator> m_authenticators;
};
}
#endif
