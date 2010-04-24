/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtupnpserviceintrospection.h
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

#ifndef QTUPNPSERVICEINTROSPECTION_H
#define QTUPNPSERVICEINTROSPECTION_H

#include <QObject>

namespace QUPnP
{
class QtUpnpServiceIntrospection : public QObject
{
public:
    QtUpnpServiceIntrospection(const QDomDocument &scpd, QObject *parent = 0);

    QList<QtUpnpServiceAction> actions() const {
        return m_actions;
    }
    QList<QtUpnpServiceStateVariable> stateVariables() const {
        return m_stateVariables;
    }

    QtUpnpServiceAction action(const QString &name);
    QtUpnpServiceStateVariable stateVariable(const QString &name);

private:
    QList<QtUpnpServiceAction> m_actions;
    QList<QtUpnpServiceStateVariable> m_stateVariables;
};
}

#endif // QTUPNPSERVICEINTROSPECTION_H
