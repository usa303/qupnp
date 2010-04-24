/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtupnpdeviceinfo.h
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

#ifndef QTUPNPDEVICEINFO_H
#define QTUPNPDEVICEINFO_H

#include <QObject>
#include <QDomElement>

namespace QUPnP
{
class QtUpnpServiceInfo;
class QtUpnpResourceFactory;
class QtUpnpContext;

class QtUpnpDeviceInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString deviceType READ deviceType WRITE setDeviceType)
    Q_PROPERTY(QString friendlyName READ friendlyName WRITE setFriendlyName)
    Q_PROPERTY(QString manufacturer READ manufacturer WRITE setManufacturer)
    Q_PROPERTY(QString manufacturerURL READ manufacturerURL WRITE setManufacturerURL)
    Q_PROPERTY(QString modelDescription READ modelDescription WRITE setModelDescription)
    Q_PROPERTY(QString modelName READ modelName WRITE setModelName)
    Q_PROPERTY(QString modelNumber READ modelNumber WRITE setModelNumber)
    Q_PROPERTY(QUrl modelURL READ modelURL WRITE setModelURL)
    Q_PROPERTY(QString serialNumber READ serialNumber WRITE setSerialNumber)
    Q_PROPERTY(QString UDN READ UDN WRITE setUDN)
    Q_PROPERTY(QString UPC READ UPC WRITE setUPC)
    Q_PROPERTY(QString iconList READ iconList WRITE setIconList)
    Q_PROPERTY(QString serviceList READ serviceList WRITE setServiceList)
    Q_PROPERTY(QString deviceList READ deviceList WRITE setDeviceList)
    Q_PROPERTY(QString presentationURL READ presentationURL WRITE setPresentationURL)

public:
//    struct DeviceIcon {
//        QString mime_type;
//        int      width;
//        int      height;
//        int      depth;
//        QString url;
//
//        int      weight;
//    };

    QtUpnpDeviceInfo(QObject * parent = 0);
    QtUpnpDeviceInfo(QtUpnpResourceFactory *factory, QtUpnpContext *context, const QDomElement &element, QObject * parent = 0);
    virtual ~QtUpnpDeviceInfo();

    QtUpnpResourceFactory* factory() const { return m_factory; }
    void setFactory(QtUpnpResourceFactory * factory) { m_factory = factory; }

    QtUpnpContext* context() const { return m_context; }
    void setContext(QtUpnpContext* context) { m_context = context; }

    QDomElement deviceDescriptionDoc() const { return m_element; }
    void setDeviceDescriptionDoc(const QDomElement &element) { m_element = element; }

    QString deviceType() const {
        return m_element.firstChildElement("deviceType").text();
    }

    QString friendlyName() const {
        return m_element.firstChildElement("friendlyName").text();
    }

    QString manufacturer() const {
        return m_element.firstChildElement("manufacturer").text();
    }

    QString manufacturerUrl() const {
        return m_element.firstChildElement("manufacturerUrl").text();
    }

    QString modelDescription() const {
        return m_element.firstChildElement("modelDescription").text();
    }

    QString modelName() const {
        return m_element.firstChildElement("modelName").text();
    }

    QString modelNumber() const {
        return m_element.firstChildElement("modelNumber").text();
    }

    QString modelUrl() const {
        return m_element.firstChildElement("modelUrl").text();
    }

    QString serialNumber() const {
        return m_element.firstChildElement("serialNumber").text();
    }

    QString presentationURL() const {
        return m_element.firstChildElement("presentationURL").text();
    }

    QString udn() const {
        return m_element.firstChildElement("UDN").text();
    }

    QString upc() const {
        return m_element.firstChildElement("UPC").text();
    }

    // TODO: currently just return icon list and give the chance to choose the best icon
    QString iconList() const {
        return m_element.firstChildElement("iconList").text();
    }

    //QList<QString> listDlnaCapabilities();
//    const QString descriptionValue(const QDomElement &element) const;

    QList<QtUpnpDeviceInfo *> deviceList() const {
        return m_deviceList;
    }

//    QStringList deviceTypes() const;
    QtUpnpDeviceInfo* device(const QString &type);

    QList<QtUpnpServiceInfo *> serviceList() const {
        return m_serviceList;
    }

//    QStringList serviceTypes() const;
    QtUpnpServiceInfo* service(const QString &type);

    //const QDomDocument document() const { return m_document; }

protected:
    virtual QtUpnpDeviceInfo* device(const QDomElement &element) = 0;
    virtual QtUpnpServiceInfo* service(const QDomElement &element) = 0;

private:
//    QString m_deviceType;
//    QString m_friendlyName;
//    QString m_manufacturer;
//    QString m_manufacturerUrl;
//    QString m_modelDescription;
//    QString m_modelName;
//    QString m_modelNumber;
//    QString m_modelUrl;
//    QString m_serialNumber;
//    QString m_presentationURL;
//    QString m_udn;
//    QString m_upc;
//    QString m_iconList;
    bool resourceTypeMatch(const QString &resource, const QString &type);

    QList<QtUpnpDeviceInfo *> m_deviceList;
    QList<QtUpnpServiceInfo *> m_serviceList;

    QDomElement m_element;
    QtUpnpResourceFactory *m_factory;
    QtUpnpContext *m_context;
};
}
#endif
