/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtupnpserviceactionargument.h
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

#ifndef QTUPNPSERVICEACTIONARGUMENT_H
#define QTUPNPSERVICEACTIONARGUMENT_H

namespace QUPnP
{
class QtUpnpServiceActionArgument : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(Direction direction READ direction WRITE setDirection)
    Q_PROPERTY(bool retval READ retval WRITE setRetval)
    Q_PROPERTY(QString relatedStateVariable READ relatedStateVariable WRITE setRelatedStateVariable)
    Q_PROPERTY(QString value READ value WRITE setValue)
    Q_ENUMS(Direction)

public:
    enum Direction {
        Input,
        Output
    }

    QtUpnpServiceActionArgument(QObject * parent = 0);
    QtUpnpServiceActionArgument(const QString name, Direction direction, bool retval, const QString relatedStateVariable, QObject * parent = 0);
    QtUpnpServiceActionArgument(const QString name, Direction direction, const QString relatedStateVariable, QObject * parent = 0);
    ~QtUpnpServiceActionArgument();

    const QString name() const {
        return m_name;
    }
    void setName(const QString);

    Direction direction() const {
        return m_direction;
    }
    void setDirection(Direction);

    bool retval() const {
        return m_retval;
    }
    void setRetval(bool);

    const QString relatedStateVariable() const {
        return m_relatedStateVariable;
    }
    void setRelatedStateVariable(const QString);

    const QString value() const {
        return m_value;
    }
    void setValue(const QString);

    static isArgument() const;

    bool operator==(const QString);
    bool operator!=(const QString);

private:
    QString m_name;
    Direction m_direction;
    bool m_retval;
    QString m_relatedStateVariable;
    QString m_value;
};

QDebug operator<<(QDebug dbg, const QtUpnpServiceActionArgument &a);
}

#endif // QTUPNPSERVICEACTIONARGUMENT_H
