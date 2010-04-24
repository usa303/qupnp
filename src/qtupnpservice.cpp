/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtupnpservice.cpp
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

#include "qtupnpservice.h"
#include "qthttpserver.h"

#include <QUdpSocket>
#include <QNetworkAccessManager>

using namespace QUPnP;

QtUpnpService::QtUpnpService(QObject * parent)
    : QtUpnpServiceInfo(parent)
{

}

QtUpnpService::QtUpnpService(QtUpnpContext *context, const QDomElement &element, QtUpnpRootDevice *rootDevice, QObject * parent)
    : QtUpnpServiceInfo(context, element, parent)
    , m_rootDevice(rootDevice)
{
    /* Get introspection and save state variable names */
    /* this will call gotIntrospection() internally */
    loadSpcd();

    /* Create soap invocation handler */
    m_soapHandler = new QtSoapInvocationHandler(this);
    connect(m_soapHandler, SIGNAL(invocationReceived(QtUpnpServiceAction *)), this, SIGNAL(actionInvoked(QtUpnpServiceAction *)));

    /* Create gena subscription handler */
    m_genaHandler = new QtGenaSubscriptionHandler(this);
    connect(m_genaHandler, SIGNAL(subsriptionReceived(Subscription *)), this, SLOT(subsriptionReceived(Subscription *)));
//    connect(m_genaHandler, SIGNAL(renewingReceived()), this, SLOT(resubscribe()));
//    connect(m_genaHandler, SIGNAL(unsubscriptionReceived()), this, SLOT(unsubscribe()));

    /* Get server */
    QtHttpServer *server = context()->httpServer();

    /* Run listener on controlURL */
    server->addHandler(controlUrl(), m_soapHandler);

    /* Run listener on eventSubscriptionURL */
    server->addHandler(subscriptionUrl(), m_genaHandler);
}

QtUpnpService::~QtUpnpService()
{
}

void QtUpnpService::gotIntrospection()
{
    if (introspection()) {
        foreach(QtUpnpServiceStateVariable *variable, introspection()->stateVariables()) {
            if (!varaible->sendEvnets())
                continue;

            m_stateVariables.append(variable->name());
        }
    } else {
        qWarning() << "Failed to get SCPD:"
                            << "The initial event message will not be sent.";
    }

    foreach (Subscription *sub, m_subscriptions)
        sendInitialState(sub);
}

void subscriptionReceived(Subscription *subscription)
{
    sendInitialState(subscription);
}

void QtUpnpService::sendInitialState(Subscription *sub)
{
    /* Send initial event message */
    // TODO: create initial eventing queue
    QString propertySet = createPropertySet();

    notifySubscriber(sub, propertySet);
}

void QtUpnpService::setRootDevice(QtUpnpRootDevice *rootDevice)
{
    if (m_rootDevice)
        disconnect(rootDevice, SIGNAL(availableChanged()), this, SLOT(rootDeviceUnavailable()));

    m_rootDevice = rootDevice;
    connect(rootDevice, SIGNAL(availableChanged()), this, SLOT(rootDeviceUnavailable()));
}

void QtUpnpService::rootDeviceUnavailable()
{
    if (!m_rootDevice->isAvailable())
        /* Root device now unavailable: Purge subscriptions */
        qDeleteAll(m_subscriptions);
}

void QtUpnpService::notify(QtUpnpServiceStateVariable *variable)
{
    /* Queue */
    m_notifyQueue.enqueue(variable);

    /* And flush, if not frozen */
    if (!m_notifyFrozen)
        flushNotifications();
}

void QtUpnpService::notify(QList<QtUpnpServiceStateVariable *> variables)
{
    foreach(QtUpnpServiceStateVariable *variable, variables)
        notify(variable);
}

void QtUpnpService::thawNotify()
{
    m_notifyFrozen = false;

    flushNotifications();
}

void QtUpnpService::flushNotifications()
{
    /* Create property set */
    QString propertySet = createPropertySet(m_notifyQueue);

    /* And send it off */
    foreach (Subscription *sub, m_subscriptions)
        notifySubscriber(sub, propertySet);
}

QString QtUpnpService::createPropertySet(QQueue<QtUpnpServiceStateVariable *>)
{
    /* Compose property set */

}

//TODO: notify should provide two methods: http or udp multicast
void QtUpnpService::notifySubscriber(Subscription *sub, const QString &propertySet)
{    
    /* Create message */
    //TODO: try callback list one by one until success
    // here use first url temporarily
    QString callback = sub->callbacks.first();
    QtHttpHeader header;
    header.setRequest(GENA_METHOD_NOTIFY, callback);
    header.addValue("NT", "upnp:event");
    header.addValue("NTS", "upnp:propchange");
    header.addValue("SID", sub->sid);
    header.addValue("SEQ", QString::number(sub->seq));
    header.addValue("NT", "upnp:event");

    header.setContentType("text/xml; charset=utf-8");

    /* Handle overflow */
    if (sub->seq < MAXINT32)
            sub->seq++;
    else
            sub->seq = 1;

    /* Create response including header and body */
    QString response = header.toString() + "\r\n\r\n" + propertySet;

    /* send notify request */
    QNetworkAccessManager *manager = m_context->httpClient();
    connect(manager, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(notifyResponseReceived(QNetworkReply*)));

    // TODO: Qt 4.7 API
    m_pendingNotifications.append(manager->sendCustomRequest(QNetworkRequest(QUrl(callback)), GENA_METHOD_NOTIFY, response));
}

void QtUpnpService::notifyResponseReceived(QNetworkReply* reply)
{
}

//void QtUpnpService::invocationReceived(QtUpnpServiceAction *action)
//{
//}
