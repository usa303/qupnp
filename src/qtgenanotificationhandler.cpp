/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtgenanotificationhandler.cpp
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

#include "qtgenanotificationhandler.h"

#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>

using namespace QUPnP;

QtGenaNotificationHandler::QtGenaNotificationHandler()
{
}

void QtGenaNotificationHandler::handle()
{
    if(m_requestHeader.method() != GENA_METHOD_NOTIFY) {
        /* We don't implement this method */
        m_responseHeader.setStatusLine(HTTP_NOT_IMPLEMENTED);

        return;
    }

    if(m_requestHeader.value("NT") != "upnp:event") {
        /* Proper NT header lacking */
        m_responseHeader.setStatusLine(HTTP_PRECONDITION_FAILED);

        return;
    }

    if(m_requestHeader.value("NTS") != "upnp:propchange") {
        /* Proper NTS header lacking */
        m_responseHeader.setStatusLine(HTTP_PRECONDITION_FAILED);

        return;
    }

    if(!m_requestHeader.hasKey("SEQ")) {
        /* No SEQ header */
        m_responseHeader.setStatusLine(HTTP_PRECONDITION_FAILED);

        return;
    }

    m_seq = QString::number(m_requestHeader.value("SEQ"));

    if(!m_requestHeader.hasKey("SID")) {
        /* No SID */
        m_responseHeader.setStatusLine(HTTP_PRECONDITION_FAILED);

        return;
    }

    m_sid = m_requestHeader.value("SID");

    /* Parse the actual XML message content */
    QDomDocument doc;
    if(!doc.setContent(m_requestBody, true)) {
        qWarning() << "Failed to parse NOTIFY message body";
        m_responseHeader.setStatusLine(HTTP_INTERNAL_SERVER_ERROR);

        return;
    }

    QDomElement element = doc.documentElement();
    if(element.tagName() != "propertyset") {
        /* Empty or unsupported */
        m_responseHeader.setStatusLine(HTTP_OK);

        return;
    } else {
        for(QDomNode n = element.firstChild(); !n.isNull(); n = n.nextSibling()) {
            if(n.tagName() == "property") {
                QtUpnpServiceStateVariable variable;
                QDomElement property = n.firstChild().toElement();
                variable.setName(property.tagName());
                variable.setValue(property.text());

                m_stateVariables.append(variable);
            }
        }
    }

    /*
     * Some UPnP stacks (hello, myigd/1.0) block when sending a NOTIFY, so
     * call the callbacks in an idle handler so that if the client calls the
     * device in the notify callback the server can actually respond.
     */
    /* pending seq/sid/stateVariables to notificationReceived slot in Class QtUpnpServiceProxy */
    // TODO: moved to class QtUpnpServiceProxy

    /* Everything went OK */
    m_responseHeader.setStatusLine(HTTP_OK);

    emit notificationReceived();
}

QByteArray QtGenaNotificationHandler::result()
{
    return m_responseHeader.toString().toAscii();
}
