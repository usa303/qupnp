/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtupnpdevice.h
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

#ifndef QTUPNPDEVICE_H
#define QTUPNPDEVICE_H

#include "qtupnpdeviceinfo.h"

namespace QUPnP
{
class QtUpnpServiceInfo;
class QtUpnpRootDevice;

class QtUpnpDevice : public QtUpnpDeviceInfo
{
public:
    QtUpnpDevice(QObject * parent = 0);
    QtUpnpDevice(QtUpnpResourceFactory *factory, QtUpnpContext *context, const QDomElement &element, QtUpnpRootDevice *rootDevice, QObject * parent = 0);
    ~QtUpnpDevice();

    QtUpnpRootDevice* rootDevice() const { return m_rootDevice; }
    void setRootDevice(QtUpnpRootDevice *rootDevice) { m_rootDevice = rootDevice; }

protected:
    QtUpnpDeviceInfo* device(const QDomElement &element);
    QtUpnpServiceInfo* service(const QDomElement &element);

private:
    QtUpnpRootDevice *m_rootDevice;
};
}
#endif
