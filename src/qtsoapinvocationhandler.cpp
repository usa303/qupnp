/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtsoapinvocationhandler.cpp
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

#include "qtsoapinvocationhandler.h"

#include <QDomDocument>

using namespace QUPnP;

QtSoapInvocationHandler::QtSoapInvocationHandler(QObject * parent)
    : QtHttpServerHandlerInterface(parent)
{
}

void QtSoapInvocationHandler::handle()
{
    if(m_requestHeader.method() != HTTP_POST) {
        m_responseHeader.setStatusLine(HTTP_NOT_IMPLEMENTED);

        return;
    }

    /* content length */

    /* Get action name */
    if(!m_requestHeader.hasKey("SOAPAction")) {
        m_responseHeader.setStatusLine(HTTP_PRECONDITION_FAILED);

        return;
    }
    QString actionName = m_requestHeader.value("SOAPAction").section('#', 1).remove('"');

    if(actionName.isEmpty()) {
        m_responseHeader.setStatusLine(HTTP_PRECONDITION_FAILED);

        return;
    }

    /* Parse action_node */
    QDomDocument doc;
    doc.setContent(m_requestBody);
    if(doc.isNull()) {
        m_responseHeader.setStatusLine(HTTP_BAD_REQUEST);

        return;
    }

    // TODO: create action
    QDomNodeList list = doc.documentElement().firstChildElement("Body").firstChildElement(actionName).childNodes();
    delete m_action;
    m_action = new QtUpnpServiceAction;

    /* Emit signal. Handler parses request and fills in
                             * response. */
    emit invocationReceived(action);

    /* No handlers attached. */
    // TODO: move to class QtUpnpService 401 "Invalid Action"
}

QByteArray QtSoapInvocationHandler::result() const
{
    //TODO: use chunked encoding or CONTENT-LENGTH header
    return m_responseHeader.toString() + actionResponse();
}

//void QtSoapInvocationHandler::setError()
//{
//
//}

QByteArray QtSoapInvocationHandler::actionResponse() const
{
    //TODO: see gupnp-service.c : new_action_response_str()
}
