/**
   @file abstractsensor_i.h
   @brief Base class for sensor interface

   <p>
   Copyright (C) 2009-2010 Nokia Corporation

   @author Joep van Gassel <joep.van.gassel@nokia.com>
   @author Semi Malinen <semi.malinen@nokia.com
   @author Timo Rongas <ext-timo.2.rongas@nokia.com>
   @author Antti Virtanen <antti.i.virtanen@nokia.com>

   This file is part of Sensord.

   Sensord is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License
   version 2.1 as published by the Free Software Foundation.

   Sensord is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with Sensord.  If not, see <http://www.gnu.org/licenses/>.
   </p>
 */

#ifndef ABSTRACTSENSOR_I_H
#define ABSTRACTSENSOR_I_H

#include <QtDBus/QtDBus>
#include <QList>
#include <QVector>
#include <QString>

#include "sfwerror.h"
#include "serviceinfo.h"
#include "socketreader.h"
#include "datatypes/datarange.h"

/*
 * Proxy class for interface local.Sensor
 */
class AbstractSensorChannelInterface : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(AbstractSensorChannelInterface)
    Q_PROPERTY(int sessionId READ sessionId)
    Q_PROPERTY(SensorError errorCode READ errorCode)
    Q_PROPERTY(QString errorString READ errorString)
    Q_PROPERTY(QString description READ description)
    Q_PROPERTY(QString id READ id)
    Q_PROPERTY(int interval READ interval WRITE setInterval)
    Q_PROPERTY(bool standbyOverride READ standbyOverride WRITE setStandbyOverride)
    Q_PROPERTY(QString type READ type)
    Q_PROPERTY(int errorCodeInt READ errorCodeInt)
    Q_PROPERTY(unsigned int bufferInterval READ bufferInterval WRITE setBufferInterval)
    Q_PROPERTY(unsigned int bufferSize READ bufferSize WRITE setBufferSize)
    Q_PROPERTY(bool hwBuffering READ hwBuffering)
    Q_PROPERTY(bool downsampling READ downsampling WRITE setDownsampling)

public:
    virtual ~AbstractSensorChannelInterface();

    bool release();

    int sessionId() const;

    SensorError errorCode();

    QString errorString();

    QString description();

    QString id();

    int interval();
    void setInterval(int value);

    bool standbyOverride();
    bool setStandbyOverride(bool override);

    unsigned int bufferInterval();
    void setBufferInterval(unsigned int value);

    bool downsampling();
    bool setDownsampling(bool value);

    /**
     * Returns list of available buffer intervals.
     *
     * @return The list of supported buffer intervals.
     */
    IntegerRangeList getAvailableBufferIntervals();

    unsigned int bufferSize();
    void setBufferSize(unsigned int value);

    /**
     * Returns list of available buffer sizes. The list is ordered by
     * efficiency of the size.
     *
     * @return The list of supported buffer sizes ordered by efficiency.
     */
    IntegerRangeList getAvailableBufferSizes();

    QString type();

    virtual QDBusReply<void> start();
    virtual QDBusReply<void> stop();

    /**
     * Get the list of available intervals for the sensor.
     *
     * @return List of available intervals (or interval ranges)
     */
    DataRangeList getAvailableIntervals();

    DataRangeList getAvailableDataRanges();
    DataRange getCurrentDataRange();
    void requestDataRange(DataRange range);
    void removeDataRangeRequest();
    bool setDataRangeIndex(int dataRangeIndex);

    /**
     * Does the sensor driver support buffering or not.
     *
     * @return Does the sensor driver support buffering or not.
     */
    bool hwBuffering();

    bool isValid() const;

private:
    int errorCodeInt();
    void setError(SensorError errorCode, const QString& errorString);
    void clearError();

protected Q_SLOTS:
    QDBusReply<void> setInterval(int sessionId, int value);
    QDBusReply<bool> setStandbyOverride(int sessionId, bool value);
    QDBusReply<void> setBufferInterval(int sessionId, unsigned int value);
    QDBusReply<void> setBufferSize(int sessionId, unsigned int value);
    QDBusReply<void> setDownsampling(int sessionId, bool value);

private Q_SLOTS: // METHODS
    QDBusReply<void> start(int sessionId);
    QDBusReply<void> stop(int sessionId);

    void dataReceived();

Q_SIGNALS: // SIGNALS
    void propertyChanged(const QString& name);

protected:
    AbstractSensorChannelInterface(const QString& path, const char* interfaceName, int sessionId);

    bool read(void* buffer, int size);

    template<typename T>
    bool read(QVector<T>& values);

    virtual bool dataReceivedImpl() = 0;

    template<typename T>
    T getAccessor(const char* name);

    template<typename T>
    void setAccessor(const char* name, const T& value);

    QDBusMessage call(QDBus::CallMode mode,
                      const QString& method,
                      const QVariant& arg1 = QVariant(),
                      const QVariant& arg2 = QVariant(),
                      const QVariant& arg3 = QVariant(),
                      const QVariant& arg4 = QVariant(),
                      const QVariant& arg5 = QVariant(),
                      const QVariant& arg6 = QVariant(),
                      const QVariant& arg7 = QVariant(),
                      const QVariant& arg8 = QVariant());

    QDBusMessage callWithArgumentList(QDBus::CallMode mode, const QString& method, const QList<QVariant>& args);

    void dbusConnectNotify(const char* signal);

private:
    struct AbstractSensorChannelInterfaceImpl;

    AbstractSensorChannelInterfaceImpl* pimpl_;

    SocketReader& getSocketReader() const;
};

template<typename T>
bool AbstractSensorChannelInterface::read(QVector<T>& values)
{
    return getSocketReader().read(values);
}

template<typename T>
T AbstractSensorChannelInterface::getAccessor(const char* name)
{
    QDBusReply<T> reply(call(QDBus::Block, QLatin1String(name)));
    if(!reply.isValid())
    {
        qDebug() << "Failed to get '" << name << "' from sensord: " << reply.error().message();
        return T();
    }
    return reply.value();
}

template<typename T>
void AbstractSensorChannelInterface::setAccessor(const char* name, const T& value)
{
    QDBusReply<void> reply(call(QDBus::Block, QLatin1String(name), qVariantFromValue(value)));
    if(!reply.isValid())
    {
        qDebug() << "Failed to set '" << name << " = " << value << "' to sensord: " << reply.error().message();
    }
}

namespace local {
  typedef ::AbstractSensorChannelInterface AbstractSensor;
}

#endif
