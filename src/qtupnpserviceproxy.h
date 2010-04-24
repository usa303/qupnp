/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtupnpserviceproxy.h
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

#ifndef QTUPNPSERVICEPROXY_H
#define QTUPNPSERVICEPROXY_H

#include "qtupnpserviceinfo.h"

namespace QUPnP
{
class QtGenaNotificationHandler;
class QtUpnpServiceAction;
class QtUpnpServiceStateVariable;

// this class is a base class for developers
class QtUpnpServiceProxy : public QtUpnpServiceInfo
{
    Q_OBJECT

public:
    QtUpnpServiceProxy(QObject * parent = 0);
    QtUpnpServiceProxy(QtUpnpContext *context, const QDomElement &element, QObject * parent = 0);
    ~QtUpnpServiceProxy();

    int subscribe();
    int renew();
    int unsubscribe();

    bool sendAction(QtUpnpServiceAction *action);

Q_SIGNALS:
    void notificationReceived(QtUpnpServiceStateVariable *stateVariable);
    void actionFinished(QtUpnpServiceAction *action);

private Q_SLOTS:
    void invocationResponseReceived();
    void notificationReceived();
    void subscriptionResponseReceived();
    void unsubscriptionResponseReceived();
    void renewingResponseReceived();

private:
    void parseInvocationResponse(const QString &response);

    bool m_isSubscribed;
    QtGenaNotificationHandler *m_genaHandler;

    QList<QNetworkReply *> m_pendingActions;
};
}
#endif
