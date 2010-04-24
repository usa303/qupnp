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

#ifndef QTUPNPRESOURCEFACTORY_H
#define QTUPNPRESOURCEFACTORY_H

#include <QObject>
#include <QHash>

class QDomElement;

//TODO: thread-safe
namespace QUPnP
{
class QtUpnpDevice;
class QtUpnpDeviceProxy;
class QtUpnpService;
class QtUpnpServiceProxy;
class QtUpnpContext;

class QtUpnpResourceFactory : public QObject
{
public:
    static QtUpnpResourceFactory* instance(QObject *parent = 0) const {
        if (!m_instance)
            return new QtUpnpResourceFactory(parent);

        return m_instance;
    }

    QtUpnpDevice* createDevice(QtUpnpContext *context, const QDomElement &element);
    QtUpnpDeviceProxy* createDeviceProxy(QtUpnpContext *context, const QDomElement &element);
    QtUpnpService* createService(QtUpnpContext *context, const QDomElement &element);
    QtUpnpServiceProxy* createServiceProxy(QtUpnpContext *context, const QDomElement &element);

    void registerResourceType(const QString &upnpTtype, int type);
    void unregisterResourceType(const QString &upnpTtype);
    void registerResourceProxyType(const QString &upnpTtype, int type);
    void unregisterResourceProxyType(const QString &upnpTtype);

private:
    QtUpnpResourceFactory(QObject *parent);

    QHash<QString, int> m_resourceTypes;
    QHash<QString, int> m_resourceProxyTypes;
    static QtUpnpResourceFactory *m_instance;
};
}

#endif // QTUPNPRESOURCEFACTORY_H
