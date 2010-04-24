/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtgenasubscriptionhandler.cpp
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

#include "qtgenasubscriptionhandler.h"

#include <QSignalMapper>
#include <QUuid>

using namespace QUPnP;

QtGenaSubscriptionHandler::QtGenaSubscriptionHandler(QObject * parent)
    : QtHttpServerHandlerInterface(parent)
    , m_subscription(NULL)
{
    m_signalMapper = new QSignalMapper(this);
    connect(m_signalMapper, SIGNAL(mapped(const QString &)),
            this, SLOT(subscriptionTimeout(const QString &)));
}

void QtGenaSubscriptionHandler::handle()
{
    /* Choose appropriate signal */
    if(m_requestHeader.method() == GENA_METHOD_SUBSCRIBE) {
        if(m_requestHeader.hasKey("Callback")) {
            if(m_requestHeader.hasKey("SID"))
                m_responseHeader.setStatusLine(HTTP_BAD_REQUEST);
            else if(m_requestHeader.value("NT") != "upnp:event")
                m_responseHeader.setStatusLine(HTTP_PRECONDITION_FAILED);
            else {
                subscribe(m_requestHeader.value("Callback"), m_requestHeader.value("Timeout").toInt());
            }
        } else if(m_requestHeader.hasKey("SID")) {
            if(m_requestHeader.hasKey("NT"))
                m_responseHeader.setStatusLine(HTTP_BAD_REQUEST);
            else {
                resubscribe(m_requestHeader.value("SID"), m_requestHeader.value("Timeout").toInt());
            }
        } else {
            m_responseHeader.setStatusLine(HTTP_BAD_REQUEST);
        }
    } else if(m_requestHeader.method() == GENA_METHOD_UNSUBSCRIBE) {
        if(m_requestHeader.hasKey("SID")) {
            if(m_requestHeader.hasKey("NT") || m_requestHeader.hasKey("Callback"))
                m_responseHeader.setStatusLine(HTTP_BAD_REQUEST);
            else {
                unsubscribe(m_requestHeader.value("SID"));
            }
        } else {
            m_responseHeader.setStatusLine(HTTP_PRECONDITION_FAILED);
        }
    } else {
        m_responseHeader.setStatusLine(HTTP_NOT_IMPLEMENTED);
    }
}

QByteArray QtGenaSubscriptionHandler::result() const
{
    return m_responseHeader.toString().toAscii();
}

void QtGenaSubscriptionHandler::subscribe(QString callbacks, int time)
{
    Subscription *sub = new Subscription;

    /* Parse callback list */
    sub->callbacks = parseCallbacks(callbacks);

    /* Add service and SID */
    sub->sid = QUuid::createUuid();

    /* Add timeout */
    int time_out = actualTimeout(time);
    if (time_out > 0) {
        QTimer *timer = new QTimer;
        connect(timer, SIGNAL(timeout()), m_signalMapper, SLOT(map()));
        m_signalMapper->setMapping(timer, sub->sid);

        sub->timer = timer;
        timer->start(time_out);
    }

    /* Add to hash */
    m_subscriptions.insert(sub->sid, sub);

    /* Respond */
    subscriptionResponse(sub->sid, time_out);

    // record the new subscription which is used to notify service to send initial state
    m_newSubscription = sub;
}

void QtGenaSubscriptionHandler::unsubscribe(const QString &sid)
{
    if (m_subscriptions.contains(sid)) {
        delete m_subscriptions.value(sid);
        m_subscriptions.remove(sid);

        m_responseHeader.setStatusLine(HTTP_OK);
    } else {
        m_responseHeader.setStatusLine(HTTP_PRECONDITION_FAILED);
    }
}

void QtGenaSubscriptionHandler::resubscribe(const QString &sid, int time)
{
    if (!m_subscriptions.contains(sid)) {
        m_responseHeader.setStatusLine(HTTP_PRECONDITION_FAILED);

        return;
    }

    Subscription *sub = m_subscription.value(sid);

    /* Update timeout */
    int time_out = actualTimeout(time);
    if (time_out > 0) {
        if (sub->timer) {
            sub->timer->stop();
        } else {
            QTimer *timer = new QTimer;
            connect(timer, SIGNAL(timeout()), m_signalMapper, SLOT(map()));
            m_signalMapper->setMapping(timer, sub->sid);

            sub->timer = timer;
        }

        sub->timer->start(time_out);
    }


    /* Respond */
    subscriptionResponse(sub->sid, time_out);
}

void QtGenaSubscriptionHandler::afterResponse()
{
    // if a new subscription is created, then notify service to send initial state
    if (m_newSubscription) {
        emit subsriptionReceived(m_newSubscription);

        m_newSubscription = NULL;
    }
}

int QtGenaSubscriptionHandler::actualTimeout(int timeout)
{

}

QStringList QtGenaSubscriptionHandler::parseCallbacks(const QString &callbacks)
{

}

void QtGenaSubscriptionHandler::subscriptionResponse(const QString &sid, int timeout)
{
    /* SID header */
    m_responseHeader.addValue("SID", sid);

    /* Timeout header */
    if (timeout > 0)
        m_responseHeader.addValue("Timeout", QString("Second-") + QString::number(timeout));
    else
        m_responseHeader.addValue("Timeout", "infinite");

    /* 200 OK */
    m_responseHeader.setStatusLine(HTTP_OK);
}

void QtGenaSubscriptionHandler::subscriptionTimeout(const QString &sid)
{
    delete m_subscriptions.value(sid);
    m_subscriptions.remove(sid);
}
