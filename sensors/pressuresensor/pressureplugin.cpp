/**
   @file PressurePlugin.cpp
   @brief Plugin for ALSSensor

   <p>
   Copyright (C) 2016 Canonical LTD.

   @author Lorn Potter <lorn.potter@canonical.com>

   This file is part of Sensorfw.

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

#include "pressureplugin.h"
#include "pressuresensor.h"
#include "sensormanager.h"
#include "logging.h"

void PressurePlugin::Register(class Loader&)
{
    sensordLogD() << "registering pressuresensor";
    SensorManager& sm = SensorManager::instance();
    sm.registerSensor<PressureSensorChannel>("pressuresensor");
}

void PressurePlugin::Init(class Loader& l)
{
    Q_UNUSED(l);
    SensorManager::instance().requestSensor("pressuresensor");
}

QStringList PressurePlugin::Dependencies() {
    return QString("pressureadaptor").split(":", QString::SkipEmptyParts);
}

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(pressuresensor, PressurePlugin)
#endif
