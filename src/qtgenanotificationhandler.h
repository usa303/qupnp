/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtgenanotificationhandler.h
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

#ifndef QTGENANOTIFICATIONHANDLER_H
#define QTGENANOTIFICATIONHANDLER_H

#include "qthttpserverhandlerinterface.h"
#include "qtupnpservicestatevariable.h"

/* this class is used by QtUpnpServiceProxy */
namespace QUPnP
{
class QtGenaNotificationHandler : public QtHttpServerHandlerInterface
{
    Q_OBJECT

public:
    QtGenaNotificationHandler();

    void handle();
    QByteArray result() const;

    QString sid() const {
        return m_sid;
    }
    quint32 seq() const {
        return m_seq;
    }

    QList<QtUpnpServiceStateVariable> stateVariable() const {
        return m_stateVariables;
    }

Q_SIGNALS:
    void notificationReceived();

private:
    QString m_sid;
    quint32 m_seq;
    QList<QtUpnpServiceStateVariable> m_stateVariables;
};
}

#endif // QTGENANOTIFICATIONHANDLER_H
