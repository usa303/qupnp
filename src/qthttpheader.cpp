/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qthttpheader.cpp
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

#include "qthttpheader.h"

#include <QStringList>

using namespace QUPnP;

QtHttpHeader::QtHttpHeader(HeaderType type)
    : QHttpHeader()
    , m_type(type)
{
}

QtHttpHeader::QtHttpHeader(const QString &header)
    : QHttpHeader(header.section("\r\n", 1))
{
    int len = header.length() - QString("\r\n").length() - header.section("\r\n", 1).length();
    QString str = header.left(len);
    parseFristLine(str);
}

void    QtHttpHeader::setRequest(const QString & method, const QString & path, int majorVer, int minorVer)
{
    if(m_type != Request)
        return;

    m_method = method;
    m_path = path;
    m_majorVersion = majorVer;
    m_minorVersion = minorVer;
}

void    QtHttpHeader::setStatusLine(int code, const QString & text, int majorVer, int minorVer)
{
    if(m_type != Response)
        return;

    m_statusCode = code;
    m_reasonPhrase = text;
    m_majorVersion = majorVer;
    m_minorVersion = minorVer;
}

QString QtHttpHeader::toString() const
{
    QString str;
    switch(type()) {
    case Request:
        str.append(m_method + " " + m_path + " " + "HTTP/" + QString::number(m_majorVersion) + "." + QString::number(m_minorVersion) + "\r\n" + QHttpHeader::toString());
        break;
    case Response:
        str.append(QString("HTTP/") + QString::number(m_majorVersion) + "." + QString::number(m_minorVersion) + " " + QString::number(m_statusCode) + " " +  m_reasonPhrase + "\r\n" + QHttpHeader::toString());
        break;
    default:
        str.append(QHttpHeader::toString());
        break;
    }

    return str;
}

void    QtHttpHeader::parseFristLine(const QString &str)
{
    QStringList list = str.split(" ");
    if(str.length() != 3) {
        m_isValid = false;
        return;
    }
    m_isValid = true;

    if(list.first() == "HTTP")
        m_type = Response;
    else
        m_type = Request;

    switch(m_type) {
    case Request:
        m_method = list.at(0);
        m_path = list.at(1);
        m_majorVersion = QString::number(list.at(2)[5]);
        m_minorVersion = QString::number(list.at(2)[7]);
        break;
    case Response:
        m_majorVersion = QString::number(list.at(0)[5]);
        m_minorVersion = QString::number(list.at(0)[7]);
        m_statusCode = QString::number(list.at(1));
        m_reasonPhrase = list.at(2);
        break;
    default:
        break;
    }
}

bool QtHttpHeader::isValid() const
{
    return m_isValid && QHttpHeader::isValid();
}
