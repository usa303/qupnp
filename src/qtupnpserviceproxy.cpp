/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtupnpserviceproxy.cpp
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

#include "qtupnpserviceproxy.h"

#include <QNetworkAccessManager>

using namespace QUPnP;

QtUpnpServiceProxy::QtUpnpServiceProxy(QObject * parent)
    : QtUpnpServiceInfo(parent)
{

}

QtUpnpServiceProxy::QtUpnpServiceProxy(QtUpnpContext *context, const QDomElement &element, QObject * parent)
    : QtUpnpServiceInfo(context, element, parent)
{

}

QtUpnpServiceProxy::~QtUpnpServiceProxy()
{
    /* Unsubscribe */
    /* Cancel pending actions */
    /* Cancel pending messages */
    /* Cancel pending notifications */
}

int QtUpnpServiceProxy::subscribe()
{

}

int QtUpnpServiceProxy::renew()
{

}

int QtUpnpServiceProxy::unsubscribe()
{

}

int QtUpnpServiceProxy::sendAction(QtUpnpServiceAction *action)
{
    /* Create soap request message */
    /* Arguments */
    /* Finish and send off */
    QNetworkAccessManager *manager = context()->httpClient();
    connect(manager, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(invocationResponseReceived(QNetworkReply*)));

    // TODO: Qt 4.7 API
    m_pendingActions.append(manager->post(QNetworkRequest(QUrl(controlUrl)), actionMessage));
}

void QtUpnpServiceProxy::invocationResponseReceived(QNetworkReply* reply)
{
    if (!reply->error()) {
        switch(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()) {
        case HTTP_CANCELLED:
            /* Silently return */
            break;
        // M_POST is deprecated
        default:
            /* Success: parse response */
            parseInvocationResponse(response);
            break;
        }
    }

    m_pendingActions.removeAll(reply);
    reply->deleteLater();
}

void QtUpnpServiceProxy::parseInvocationResponse(const QString &response)
{
    emit actionFinished(action);
}

void QtUpnpServiceProxy::notificationReceived()
{
}

void QtUpnpServiceProxy::subscriptionResponseReceived()
{
}

void QtUpnpServiceProxy::unsubscriptionResponseReceived()
{
}

void QtUpnpServiceProxy::renewingResponseReceived()
{
}

