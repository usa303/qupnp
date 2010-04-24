/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtupnpserviceaction.h
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

#ifndef QTUPNPSERVICEACTION_H
#define QTUPNPSERVICEACTION_H

#include <QObject>

namespace QUPnP
{
class QtUpnpServiceAction : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QList<QtUpnpServiceActionArgument*> argumentList READ argumentList WRITE setArgumentList)

public:
    QtUpnpServiceAction(QObject * parent = 0);
    QtUpnpServiceAction(const QString name, QList<QtUpnpServiceActionArgument*> argumentList, QObject * parent = 0);
    ~QtUpnpServiceAction();

    void addArgument(QtUpnpServiceActionArgument *);
    void removeArgument(QtUpnpServiceActionArgument *);
    bool hasArgument(QtUpnpServiceActionArgument *);

    QList<QtUpnpServiceActionArgument*> argumentList() const {
        return m_argumentList;
    }
    void setArgumentList(QList<QtUpnpServiceActionArgument*>);
    void clearArgumentList();

    const QString name() const {
        return m_name;
    }
    void setName(const QString);

private:
    QString m_name;
    QList<QtUpnpServiceActionArgument *> m_argumentList;

    QList<QtUpnpServiceActionArgument *> inArgumentList();
    QList<QtUpnpServiceActionArgument *> outArgumentList();
};

QDebug operator<<(QDebug dbg, const QtUpnpServiceAction &a);
}

#endif // QTUPNPSERVICEACTION_H
