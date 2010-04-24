/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qthttpheader.h
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

#ifndef QTHTTPHEADER_H
#define QTHTTPHEADER_H

#include <QHttpHeader>

/* This class is just a temparary solution since Qt 4.6 has gived up class QHttpHeader */
namespace QUPnP
{
class QtHttpHeader : public QHttpHeader
{
public:
    enum HeaderType {
        Request,
        Response
    };

    QtHttpHeader(HeaderType type);
    QtHttpHeader(const QString &header);

    HeaderType type() const {
        return m_type;
    }

    /* methods only used by request header */
    QString method() const {
        return m_method;
    }

    QString path() const {
        return m_path;
    }

    void    setRequest(const QString & method, const QString & path, int majorVer = 1, int minorVer = 1);

    /* methods only used by response header */
    QString reasonPhrase() const {
        return m_reasonPhrase;
    }

    int statusCode() const {
        return m_statusCode;
    }

    void    setStatusLine(int code, const QString & text = QString(), int majorVer = 1, int minorVer = 1);

    bool isValid() const;

    /* methods used by both request and response header */
    int majorVersion() const {
        return m_majorVersion;
    }
    int minorVersion() const {
        return m_minorVersion;
    }
    virtual QString toString() const;

private:
    HeaderType m_type;
    QString m_method;
    QString m_path;
    QString m_reasonPhrase;
    int m_statusCode;
    int m_majorVersion;
    int m_minorVersion;
    bool m_isValid;
};
}

#endif // QTHTTPHEADER_H
