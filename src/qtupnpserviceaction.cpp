/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtupnpserviceaction.cpp
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

#include "qtupnpserviceaction.h"

using namespace QUPnP;

QtUpnpServiceActionArgument::QtUpnpServiceActionArgument(QObject * parent)
    : QObject(parent)
{

}

QtUpnpServiceActionArgument::~QtUpnpServiceActionArgument()
{
}

void QtUpnpServiceActionArgument::setName(const QString name)
{
    m_name = name;
}

void QtUpnpServiceActionArgument::setDirection(Direction direction)
{
    m_direction = direction;
}

void QtUpnpServiceActionArgument::setRetval(bool retval)
{
    m_retval = retval;
}

void QtUpnpServiceActionArgument::setRelatedStateVariable(const QString stateVariable)
{
    m_relatedStateVariable = stateVariable;
}

void QtUpnpServiceActionArgument::setValue(const QString value)
{
    m_value = value;
}

QtUpnpServiceAction::QtUpnpServiceAction(QObject * parent)
    : QObject(parent)
{

}

QtUpnpServiceAction(const QString name, QList<QtUpnpServiceActionArgument*> argumentList, QObject * parent)
, QObject(parent)
, m_name(name)
, m_argumentList(argumentList)
{
}

QtUpnpServiceAction::~QtUpnpServiceAction()
{
}

void QtUpnpServiceAction::setName(const QString name)
{
    m_name = name;
}

void QtUpnpServiceAction::setArgumentList(QList<QtUpnpServiceActionArgument*> list)
{
    m_argumentList = list;
}

void QtUpnpServiceAction::clearArgumentList()
{
    m_argumentList->clear();
}

void QtUpnpServiceAction::addArgument(QtUpnpServiceActionArgument *arg)
{
    m_argumentList->append(arg);
}

void QtUpnpServiceAction::removeArgument(QtUpnpServiceActionArgument *arg)
{
    m_argumentList->removeAll(arg);
}

bool QtUpnpServiceAction::hasArgument(QtUpnpServiceActionArgument *arg)
{
    return m_argumentList->contains(arg);
}

QDebug operator<<(QDebug dbg, const QtUpnpServiceAction &a)
{
    dbg.nospace() << "QtUpnpServiceAction: (" << a.name() << ", " << (a.direction() == Input) ? "Input" : "Output" << ", " <<
                  a.retval() << ", " << a.relatedStateVariable()() << ")";

    return dbg.space();
}
