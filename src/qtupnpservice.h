/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtupnpservice.h
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

#ifndef QTUPNPSERVICE_H
#define QTUPNPSERVICE_H

#include "qtupnpserviceinfo.h"

#include <QHash>
#include <QQueue>

namespace QUPnP
{
class QtUpnpServiceAction;
class QtSoapInvocationHandler;
class QtGenaSubscriptionHandler;
class QtUpnpRootDevice;
class QtUpnpServiceStateVariable;
class Subscription;

class QtUpnpService : public QtUpnpServiceInfo
{
    Q_OBJECT

public:
    QtUpnpService(QObject * parent = 0);
    QtUpnpService(QtUpnpContext *context, const QDomElement &element, QtUpnpRootDevice *rootDevice, QObject * parent = 0);
    ~QtUpnpService();

    QtUpnpRootDevice* rootDevice() const { return m_rootDevice; }

    void notify(QtUpnpServiceStateVariable *variable);
    void notify(QList<QtUpnpServiceStateVariable *> variables);

    void freezeNotify() const { m_notifyFrozen = true; }
    void thawNotify();

protected:
    void gotIntrospection();

Q_SIGNALS:
    void actionInvoked(QtUpnpServiceAction *action);
    //TODO: emitted when notify failed.
    void notifyFailed();

private Q_SLOTS:
    void subscriptionReceived(Subscription *subscription);
    void rootDeviceUnavailable();
//    void invocationReceived(QtUpnpServiceAction *action);
//    void subscribe();
//    void unsubscribe();
//    void resubscribe();
    void notifyResponseReceived();

private:
    void flushNotifications();
    QString createPropertySet(QQueue<QtUpnpServiceStateVariable *>);
    void sendInitialState(Subscription *sub);
    void notifySubscriber(Subscription *sub, const QString &propertySet);

    QtSoapInvocationHandler *m_soapHandler;
    QtGenaSubscriptionHandler *m_genaHandler;

    QtUpnpRootDevice *m_rootDevice;

    QStringList m_stateVariables;

    QQueue<QtUpnpServiceStateVariable *> m_notifyQueue;
    bool m_notifyFrozen;

    QList<QNetworkReply *> m_pendingNotifications;
};
}
#endif
