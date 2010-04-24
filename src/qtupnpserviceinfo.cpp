/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtupnpserviceinfo.cpp
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

#include "qtupnpserviceinfo.h"
#include "qtupnpserviceintrospection.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>

using namespace QUPnP;

QtUpnpServiceInfo::QtUpnpServiceInfo(QObject * parent)
    : QObject(parent)
{

}

QtUpnpServiceInfo::QtUpnpServiceInfo(QtUpnpContext *context, const QDomElement &element, QObject * parent)
    : QObject(parent)
    , m_context(context)
    , m_element(element)
{

}

QtUpnpServiceInfo::~QtUpnpServiceInfo()
{
    /* Cancel any pending SCPD GETs */
    qDeleteAll(m_pendingRequest);
}

/* Calling this fuction causes http get request
    and creates a new QtUpnpServiceIntrospection instance each time.
    When the instance is created successfully, it will emit a signal notifying clients.
    Clients should delete this pointer by themselves.
    Don't call this fuction too much to waste network resources.
    Cache the pointer for following use.
*/
void QtUpnpServiceInfo::loadScpd()
{
    /* Get SCPD doc */
    QNetworkAccessManager *manager = m_context->httpClient();
    connect(manager, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(scpdReceived(QNetworkReply*)));

    m_pendingRequest.append(manager->get(QNetworkRequest(QUrl(scpdUrl()))));
}

void QtUpnpServiceInfo::scpdReceived(QNetworkReply* reply)
{
    if (!reply->error()) {
        QDomDocument doc;
        doc.setContent(networkReply->device(), true);

        if (!doc.isNull()) {
            m_introspection = new QtUpnpServiceIntrospection(doc);
            gotIntrospection();
        }
    }

    m_pendingRequest.removeAll(reply);
    reply->deleteLater();
}
