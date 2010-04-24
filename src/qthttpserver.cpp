/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qthttpserver.cpp
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

#include "qtupnpprotocol.h"
#include "qthttpserver.h"
#include "qthttpserverhandlerinterface.h"
#include "qthttpheader.h"

#include <QDateTime>
#include <QTcpSocket>
#include <QTextStream>
#include <QRegExp>

using namespace QUPnP;

QtHttpServer::QtHttpServer(HttpServerMode mode, QObject * parent)
    : QTcpServer(parent)
    , m_mode(mode)
    , m_disabled(false)
    , m_serverHeader("Qt UPnP")
{

}

QtHttpServer::~QtHttpServer()
{
}

void QtHttpServer::pause()
{
    m_disabled = true;
}

void QtHttpServer::resume()
{
    m_disabled = false;
}

void QtHttpServer::setMode(HttpServerMode mode)
{
    m_mode = mode;

    /* change the running mode to 'mode' */
}

void QtHttpServer::setServerHeader(const QString &header)
{
    m_serverHeader = header;
}

void QtHttpServer::addHandler(const QUrl &url, QtHttpServerHandlerInterface *handler)
{
    m_handlers.insert(url, handler);
}

void QtHttpServer::removeHandler(const QUrl &url)
{
    m_handlers.remove(url);
}

void QtHttpServer::addAuthenticator(const QAuthenticator &authenticator)
{
    m_authenticators.insert(authenticator);
}

void QtHttpServer::removeAuthenticator(const QAuthenticator &authenticator)
{
    m_authenticators.remove(authenticator);
}

void QtHttpServer::incomingConnection(int socketDescriptor)
{
    qDebug() << "New Connection";

    if(m_disabled)
        return;

    QTcpSocket* s = new QTcpSocket(this);
    connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
    s->setSocketDescriptor(socketDescriptor);
}

void QtHttpServer::readClient()
{
    // This slot is called when the client sent data to the server.
    if(disabled)
        return;

    QTcpSocket* socket = (QTcpSocket*)sender();

    while(socket->canReadLine()) {
        /* parse http request header and body respectively */
        QStringList message = QString(socket->readLine()).split(QRegExp("[ \r\n\r\n]*"));
        QtHttpHeader requestHeader(message.first());
        QString requestBody = message.last();

        QtHttpHeader responseHeader;
        /* Add required response headers */
        /* "Server:", "Date:" */
        responseHeader.addValue("Server", m_serverHeader);
        responseHeader.addValue("Date", QDateTime::currentDateTime().toString());

        /* Now handle authentication. (We do this here so that if
         * the request uses "Expect: 100-continue", we can reject it
         * immediately rather than waiting for the request body to
         * be sent.
         */
        bool authenticated = isAuthenticated(requestHeader);

        /* identify if there is handler for the requested url */
        QUrl url = requestHeader.path();
        bool hasHandler = m_handlers.contains(url);

        QTextStream os(socket);
        os.setAutoDetectUnicode(true);

        /* If no auth domain rejected it, then it's ok. */
        QtHttpServerHandlerInterface *handler = m_handlers[url];
        if(authenticated) {
            if(hasHandler) {
                handler->setRequestHeader(requestHeader);
                handler->setRequestBody(requestBody);
                handler->setResponseHeader(responseHeader);
                handler->handle();
                os << handler->result();
                handler->afterResponse();
            } else {
                defaultHandling(responseHeader);
            }
        } else {
            authenticationFailedHandling(responseHeader);
        }

        if(!authenticated || !hasHandler) {
            os << responseHeader.toString();
        }

        qDebug() << "Wrote to client";

        bool keepAlive = isKeepAlive(requestHeader, responseHeader);
        if(socket->state() == QTcpSocket::ConnectedState && keepAlive) {
            //continue to read this socket until it's not alive
            continue;
        }

        /* close socket */
        socket->close();

        if(socket->state() == QTcpSocket::UnconnectedState) {
            delete socket;
            qDebug() << "Connection closed";
        }
    }
}

void QtHttpServer::discardClient()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();

    qDebug() << "Connection closed";
}

bool QtHttpServer::isAuthenticated(const QtHttpHeader &header)
{
    //TODO: always return true at this moment
//    QAuthenticator authenticator;
//    authenticator.setUser();
//    authenticator.setPassword();
//
//    foreach(QAuthenticator auth, m_authenticators) {
//        if(authenticator == auth)
//            break;
//
//        return false;
//    }

    return true;
}

void QtHttpServer::defaultHandling(const QtHttpHeader &header)
{
    /* set status code 404-HTTP_NOT_FOUND */
    header.setStatusLine(HTTP_NOT_FOUND);
}

void QtHttpServer::authenticationFailedHandling(const QtHttpHeader &header)
{
    /* add rejected info into response message */

}

bool QtHttpServer::isKeepAlive(const QtHttpHeader &requestHeader, const QtHttpHeader &responseHeader)
{
    //TODO: always return false at this moment
    return false;
}
