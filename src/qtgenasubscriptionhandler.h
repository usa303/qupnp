/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtgenasubscriptionhandler.h
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

#ifndef QTGENASUBSCRIPTIONHANDLER_H
#define QTGENASUBSCRIPTIONHANDLER_H

#include "qthttpserverhandlerinterface.h"

class QTimer;
class QSignalMapper;

namespace QUPnP
{
struct Subscription {
    ~Subscription() {
        delete timer;
    }

    QStringList callbacks;
    QString sid;
    int seq;
    QTimer *timer;
    // I guess this is used by service to cancel any ongoing notify request if service is destroyed
    // not needed here?
//        QStringList pendingMessages;
};

/* this class is used by QtUpnpService */
class QtGenaSubscriptionHandler : public QtHttpServerHandlerInterface
{
    Q_OBJECT

public:
    QtGenaSubscriptionHandler(QObject * parent = 0);

    void handle();
    QByteArray result() const;
    void afterResponse();

    QHash<QString, Subscription *> subscriptions() const { return m_subscriptions; }

    Subscription * subscription(const QString &sid) const { return m_subscriptions.value(sid); }

Q_SIGNALS:
    void subsriptionReceived(Subscription *subscription);
//    void renewingReceived();
//    void unsubscriptionReceived();

private Q_SLOTS:
    void subscriptionTimeout(const QString &sid);

private:
    void subscribe(QString callbacks, int timeout);
    void unsubscribe(const QString &sid);
    void resubscribe(const QString &sid, int timeout);

    int actualTimeout(int timeout);
    QStringList parseCallbacks(const QString &callbacks);
    void subscriptionResponse(const QString &sid, int timeout);

    QHash<QString, Subscription *> m_subscriptions;

    QSignalMapper *m_signalMapper;

    // used to record the subscription created in subscribe()
    Subscription *m_newSubscription;
};
}

#endif // QTGENASUBSCRIPTIONHANDLER_H
