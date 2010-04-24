/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtupnpserviceinfo.h
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

#ifndef QTUPNPSERVICEINFO_H
#define QTUPNPSERVICEINFO_H

#include <QObject>
#include <QDomElement>

class QNetworkReply;

namespace QUPnP
{
class QtUpnpContext;
class QtUpnpServiceIntrospection;

class QtUpnpServiceInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString serviceType READ name WRITE setServiceType)
    Q_PROPERTY(QUuid serviceId READ name WRITE setServiceId)
    Q_PROPERTY(QUrl SCPDURL READ name WRITE setSCPDURL)
    Q_PROPERTY(QUrl controlURL READ name WRITE setControlURL)
    Q_PROPERTY(QUrl eventSubURL READ name WRITE setEventSubURL)

public:
    QtUpnpServiceInfo(QObject * parent = 0);
    QtUpnpServiceInfo(QtUpnpContext *context, const QDomElement &element, QObject * parent = 0);
    virtual ~QtUpnpServiceInfo();

    QtUpnpContext* context() const { return m_context; }
    void setContext(QtUpnpContext* context) { m_context = context; }

    QDomElement deviceDescriptionDoc() const { return m_element; }
    void setDeviceDescriptionDoc(const QDomElement &element) { m_element = element; }

    QString location() const {
        return m_location;
    }

    QString urlBase() const {
        return m_urlBase;
    }

    QString udn() const {
        return m_udn;
    }

    QString serviceType() const {
        return m_element.firstChildElement("serviceType").text();
    }

    QString id() const {
        return m_element.firstChildElement("serviceId").text();
    }

    QString scpdUrl() const {
        return m_element.firstChildElement("SCPDURL").text();
    }

    QString controlUrl() const {
        return m_element.firstChildElement("controlURL").text();
    }

    QString subscriptionUrl() const {
        return m_element.firstChildElement("eventSubURL").text();
    }

    void loadScpd();
    QtUpnpServiceIntrospection* introspection() const { return m_introspection; }

protected:
    virtual void gotIntrospection() = 0;

private Q_SLOTS:
    void scpdReceived(QNetworkReply* reply);

private:


    QtUpnpContext *m_context;

//    QString m_location;
//    QString m_udn;
//    QString m_serviceType;
//    QUrl m_urlBase;

    QDomElement m_element;
    QtUpnpContext *m_context;
    QList<QNetworkReply *> m_pendingRequest;
    QtUpnpServiceIntrospection* m_introspection;
};
}
#endif
