/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qthttpserverhandlerinterface.h
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

#ifndef QTHTTPSERVERHANDLERINTERFACE_H
#define QTHTTPSERVERHANDLERINTERFACE_H

#include "qthttpheader.h"

#include <QObject>
#include <QByteArray>

namespace QUPnP
{
class QtHttpServerHandlerInterface : public QObject
{
public:
    QtHttpServerHandlerInterface(QObject * parent = 0);
    virtual ~QtHttpServerHandlerInterface();

    virtual void handle() = 0;
    virtual QByteArray result() = 0;
    virtual void afterResponse() = 0;

    void setRequestHeader(const QtHttpHeader &header) {
        m_requestHeader = header;
    }
    void setResponseHeader(const QtHttpHeader &header) {
        m_responseHeader = header;
    }
    void setRequestBody(const QString &body) {
        m_requestBody = body;
    }

protected:
    QtHttpHeader m_requestHeader;
    QtHttpHeader m_responseHeader;
    QString m_requestBody;
};
}

#endif // QTHTTPSERVERHANDLERINTERFACE_H
