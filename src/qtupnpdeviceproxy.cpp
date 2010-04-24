/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtupnpdeviceproxy.cpp
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

#include "qtupnpdeviceproxy.h"

using namespace QUPnP;

QtUpnpDeviceProxy::QtUpnpDeviceProxy(QObject * parent)
    : QtUpnpDeviceInfo(parent)
{

}

QtUpnpDeviceProxy::QtUpnpDeviceProxy(QtUpnpResourceFactory *factory, QtUpnpContext *context, const QDomElement &element, QObject * parent)
    : QtUpnpDeviceInfo(factory, context, element, parent)
{

}

QtUpnpDeviceProxy::~QtUpnpDeviceProxy()
{

}

QtUpnpDeviceInfo* QtUpnpDeviceProxy::device(const QDomElement &element)
{
    return resourceFactory()->createDeviceProxy(context(), element);
}

QtUpnpServiceInfo* QtUpnpDeviceProxy::service(const QDomElement &element)
{
    return resourceFactory()->createServiceProxy(context(), element);
}
