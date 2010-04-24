/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtupnpresourcefactory.cpp
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

#include "qtupnpresourcefactory.h"
#include "qtupnpdevice.h"
#include "qtupnpdeviceproxy.h"
#include "qtupnpservice.h"
#include "qtupnpserviceproxy.h"
#include "qtsharedpointer.h"

#include <QMetaType>

QtUpnpResourceFactory::QtUpnpResourceFactory(QObject *parent)
    : QObject(parent)
{
}

QtUpnpDevice* QtUpnpResourceFactory::createDevice(QtUpnpContext *context, const QDomElement &element)
{
    QtUpnpDevice *device = NULL;
    if (m_resourceTypes.contains(element.firstChildElement("deviceType"))) {
        device = static_cast<QtSharedPointer<QtUpnpDevice>*>(QMetaType::construct(m_resourceTypes.value("deviceType")));

        device->setFactory(this);
        device->setContext(context);
        device->setElement(element);
    }

    return device;
}

QtUpnpDeviceProxy* QtUpnpResourceFactory::createDeviceProxy(QtUpnpContext *context, const QDomElement &element)
{
    QtUpnpDeviceProxy *deviceProxy = NULL;
    if (m_resourceProxyTypes.contains(element.firstChildElement("deviceType"))) {
        deviceProxy = static_cast<QtSharedPointer<QtUpnpDeviceProxy>*>(QMetaType::construct(m_resourceProxyTypes.value("deviceType")));

        deviceProxy->setFactory(this);
        deviceProxy->setContext(context);
        deviceProxy->setElement(element);
    }

    return deviceProxy;
}

QtUpnpService* QtUpnpResourceFactory::createService(QtUpnpContext *context, const QDomElement &element)
{
    QtUpnpService *service = NULL;
    if (m_resourceTypes.contains(element.firstChildElement("serviceType"))) {
        service = static_cast<QtSharedPointer<QtUpnpService>*>(QMetaType::construct(m_resourceTypes.value("serviceType")));

        service->setFactory(this);
        service->setContext(context);
        service->setElement(element);
    }

    return service;
}

QtUpnpServiceProxy* QtUpnpResourceFactory::createServiceProxy(QtUpnpContext *context, const QDomElement &element)
{
    QtUpnpServiceProxy *serviceProxy = NULL;
    if (m_resourceProxyTypes.contains(element.firstChildElement("serviceType"))) {
        serviceProxy = static_cast<QtSharedPointer<QtUpnpServiceProxy>*>(QMetaType::construct(m_resourceProxyTypes.value("serviceType")));

        serviceProxy->setFactory(this);
        serviceProxy->setContext(context);
        serviceProxy->setElement(element);
    }

    return serviceProxy;
}

void QtUpnpResourceFactory::registerResourceType(const QString &upnpTtype, int type)
{
    m_resourceTypes.insert(upnpTtype, type);
}

void QtUpnpResourceFactory::unregisterResourceType(const QString &upnpTtype)
{
    m_resourceTypes.remove(upnpType);
}

void QtUpnpResourceFactory::registerResourceProxyType(const QString &upnpTtype, int type)
{
    m_resourceProxyTypes.insert(upnpType, type);
}

void QtUpnpResourceFactory::unregisterResourceProxyType(const QString &upnpTtype)
{
    m_resourceProxyTypes.remove(upnpType);
}
