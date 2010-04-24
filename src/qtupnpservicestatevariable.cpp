/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtupnpservicestatevariable.cpp
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

#include "qtupnpservicestatevariable.h"

using namespace QUPnP;

QtUpnpServiceStateVariable::QtUpnpServiceStateVariable(QObject * parent)
    : QObject(parent)
{

}

QtUpnpServiceStateVariable(const QString name, const QString dataType, const QString defaultValue,
                           QStringList allowedValueList, const AllowedValueRange allowedValueRange,
                           bool sendEvents, bool multicast, QObject * parent)
, QObject(parent)
, m_name(name)
, m_dataType(dataType)
, m_defaultValue(defaultValue)
, m_allowedValueList(allowedValueList)
, m_allowedValueRange(allowedValueRange)
, m_sendEvents(sendEvents)
, m_multicast(multicast)
{
}

QtUpnpServiceStateVariable::~QtUpnpServiceStateVariable()
{
}

void QtUpnpServiceStateVariable::setName(const QString name)
{
    m_name = name;
}

void QtUpnpServiceStateVariable::setDataType(const QString dataType)
{
    m_dataType = dataType;
}

void QtUpnpServiceStateVariable::setDefaultValue(const QString defaultValue)
{
    m_defaultValue = defaultValue;
}

void QtUpnpServiceStateVariable::setValue(const QString value)
{
    m_value = value;

    emit valueChanged(m_value);
}

void QtUpnpServiceStateVariable::setAllowedValueList(const QStringList allowedValueList)
{
    m_allowedValueList = allowedValueList;
}

void QtUpnpServiceStateVariable::setAllowedValueRange(const AllowedValueRange allowedValueRange)
{
    m_nameallowedValueRange = allowedValueRange;
}

void QtUpnpServiceStateVariable::setSendEvents(bool sendEvents)
{
    m_sendEvents = sendEvents;
}

void QtUpnpServiceStateVariable::setMulticast(bool multicast)
{
    m_multicast = multicast;
}

QDebug operator<<(QDebug dbg, const QtUpnpServiceStateVariable &s)
{
    dbg.nospace() << "QtUpnpServiceActionArgument: (" << a.name() << ", " << (a.direction() == Input) ? "Input" : "Output" << ", " <<
                  a.retval() << ", " << a.relatedStateVariable()() << ")";

    return dbg.space();
}
