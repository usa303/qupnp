/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtupnpdeviceinfo.cpp
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

#include "qtupnpdeviceinfo.h"
#include "qtupnpserviceinfo.h"

using namespace QUPnP;

QtUpnpDeviceInfo::QtUpnpDeviceInfo(QObject * parent)
    : QObject(parent)
{

}

QtUpnpDeviceInfo::QtUpnpDeviceInfo(QtUpnpResourceFactory *factory, QtUpnpContext *context, const QDomElement &element, QObject * parent)
    : QObject(parent)
    , m_element(element)
    , m_factory(factory)
    , m_context(context)
{

}

QtUpnpDeviceInfo::~QtUpnpDeviceInfo()
{
    qDeleteAll(m_deviceList);
    qDeleteAll(m_serviceList);
}

//const Icon QtUpnpDeviceInfo::iconParse(const QDomElement &element)
//{
//    Icon icon;
//
//    icon.mime_type = element.elementsByTagName("mimetype").item(0).toElement().text();
//    icon.width = element.elementsByTagName("width").item(0).toElement().text();
//    icon.height = element.elementsByTagName("height").item(0).toElement().text();
//    icon.depth = element.elementsByTagName("depth").item(0).toElement().text();
//    icon.url = element.elementsByTagName("url").item(0).toElement().text();
//
//    return icon;
//}
//
//const QString QtUpnpDeviceInfo::iconUrl() const
//{
//    QDomElement iconList = m_document.elementsByTagName("iconList").item(0).toElement();
//
//    for(QDomNode n = iconList.firstChild(); !n.isNull(); n = n.nextSibling()) {
//        Icon icon = iconParse(n.toElement());
//
//        // TODO: get the best icon
//    }
//}

//QList<QString> QtUpnpDeviceInfo::listDlnaCapabilities()
//{
//
//}
//
//const QString QtUpnpDeviceInfo::descriptionValue(const QString &element) const
//{
//    return m_document.elementsByTagName(element).item(0).toElement().text();
//}

QList<QtUpnpDeviceInfo *> QtUpnpDeviceInfo::deviceList() const
{
    // TODO: this is not very correct since the deviceList is really emply in this device.
    // for this case, it's not needed to check the device list actually.
    if (m_deviceList.isEmpty()) {
        QDomNodeList nodes =  m_element.firstChildElement("deviceList").childNodes();

        for(int i = 0; i < nodes.size(); ++i) {
            m_deviceList.append(device(nodes.at(i).toElement()));
        }
    }

    return m_deviceList;
}

//QStringList QtUpnpDeviceInfo::deviceTypes() const
//{
//    QDomNodeList nodes =  m_element.firstChildElement("deviceList").childNodes();
//    QStringList list;
//
//    for(int i = 0; i < nodes.size(); ++i) {
//        list.append(nodes.at(i).toElement().firstChildElement("deviceType").text());
//    }
//
//    return list;
//}

QtUpnpDeviceInfo* QtUpnpDeviceInfo::device(const QString &type)
{
    //get the device according to device type
    foreach(QtUpnpDeviceInfo *device, deviceList()) {
        if (resourceTypeMatch(device->deviceType(), type))
            return device;
    }

    return NULL;
}

QList<QtUpnpServiceInfo *> QtUpnpDeviceInfo::serviceList() const
{
    // TODO: this is not very correct since the serviceList is really emply in this device.
    // for this case, it's not needed to check the service list actually.
    if (m_serviceList.isEmpty()) {
        QDomNodeList nodes =  m_element.firstChildElement("serviceList").childNodes();

        for(int i = 0; i < nodes.size(); ++i) {
            m_serviceList.append(service(nodes.at(i).toElement()));
        }
    }

    return m_serviceList;
}

//QStringList QtUpnpDeviceInfo::serviceTypes() const
//{
//    QDomNodeList nodes =  m_document.firstChildElement("serviceList").childNodes();
//    QStringList list;
//
//    for(int i = 0; i < nodes.size(); ++i) {
//        list.append(nodes.at(i).toElement().firstChildElement("serviceType").text());
//    }
//
//    return list;
//}

QtUpnpServiceInfo* QtUpnpDeviceInfo::service(const QString &type)
{
    //get the service according to service type
    foreach(QtUpnpServiceInfo *service, serviceList()) {
        if (resourceTypeMatch(service->serviceType(), type))
            return service;
    }

    return NULL;
}

bool QtUpnpDeviceInfo::resourceTypeMatch(const QString &resource, const QString &type)
{
    // TODO: implement it later
}
