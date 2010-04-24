/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qthttpstandardhandler.cpp
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

#include "qthttpstandardhandler.h"

using namespace QUPnP;

QtHttpStandardHandler::QtHttpStandardHandler()
{
}

void QtHttpStandardHandler::handle()
{
    if(m_requestHeader.method() != HTTP_GET && m_requestHeader.method() == HTTP_HEAD) {
        m_responseHeader.setStatusLine(HTTP_NOT_IMPLEMENTED);
        return;
    }

    /* Construct base local path */
    QString localPath = constructLocalPath(m_requestHeader.path());
    if(localPath.isEmpty()) {
        m_responseHeader.setStatusLine(HTTP_BAD_REQUEST);
        return;
    }

    /* Get preferred locales */
    QStringList locales = m_requestHeader.acceptLanguages();

}

QByteArray QtHttpStandardHandler::result() const
{

}

void QtHttpStandardHandler::hostFile(const QUrl &serverUrl, const QFileInfo &localFile)
{
    m_hostedFiles.insert(serverUrl, localPath);
}

void QtHttpStandardHandler::unhostFile(const QUrl &serverUrl)
{
    m_hostedFiles.remove(serverUrl);
}
