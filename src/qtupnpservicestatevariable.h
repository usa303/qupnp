/****************************************************************************************
*   Copyright (C) 2010 Gao Rui
*   E-mail: gaorui.2009@gmail.com
*
*   Filename: qtupnpservicestatevariable.h
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

#ifndef QTUPNPSERVICESTATEVARIABLE_H
#define QTUPNPSERVICESTATEVARIABLE_H

namespace QUPnP
{
class QtUpnpServiceStateVariable : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString dataType READ dataType WRITE setDataType)
    Q_PROPERTY(QString defaultValue READ defaultValue WRITE setDefaultValue)
    Q_PROPERTY(QString value READ value WRITE setValue)
    Q_PROPERTY(QStringList allowedValueList READ allowedValueList WRITE setAllowedValueList)
    Q_PROPERTY(AllowedValueRange allowedValueRange READ allowedValueRange WRITE setAllowedValueRange)
    Q_PROPERTY(bool sendEvents READ sendEvents WRITE setSendEvents)
    Q_PROPERTY(bool multicast READ multicast WRITE setMulticast)
    Q_ENUMS(Direction)

public:
    struct AllowedValueRange {
        QString minimum;
        QString maximum;
        QString step;
    };

    QtUpnpServiceStateVariable(QObject * parent = 0);
    QtUpnpServiceStateVariable(const QString name, const QString dataType, const QString defaultValue,
                               QStringList allowedValueList, const AllowedValueRange allowedValueRange,
                               bool sendEvents, bool multicast, QObject * parent = 0);
    ~QtUpnpServiceStateVariable();

    const QString name() const {
        return m_name;
    }
    void setName(const QString);

    const QString dataType() const {
        return m_dataType;
    }
    void setDataType(const QString);

    const QString defaultValue() const {
        return m_defaultValue;
    }
    void setDefaultValue(const QString);

    const QString value() const {
        return m_value;
    }
    void setValue(const QString);

    const QStringList allowedValueList() const {
        return m_allowedValueList;
    }
    void setAllowedValueList(const QString);

    const AllowedValueRange allowedValueRange() const {
        return m_allowedValueRange;
    }
    void setAllowedValueRange(const AllowedValueRange);

    bool sendEvents() const {
        return m_sendEvents;
    }
    void setSendEvents(bool);

    bool multicast() const {
        return m_multicast;
    }
    void setMulticast(bool);

private:
    QString m_name;
    QString m_dataType;
    QString m_defaultValue;
    QString m_value;
    QStringList m_allowedValueList;
    AllowedValueRange m_allowedValueRange;
    bool m_sendEvents;
    bool m_multicast;

signals:
    void valueChanged(const QString value);
};

QDebug operator<<(QDebug dbg, const QtUpnpServiceStateVariable &s);
}

#endif // QTUPNPSERVICESTATEVARIABLE_H
