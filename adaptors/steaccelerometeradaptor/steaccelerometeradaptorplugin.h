#ifndef STEACCELEROMETERADAPTORPLUGIN_H
#define STEACCELEROMETERADAPTORPLUGIN_H

#include "plugin.h"

class SteAccelerometerAdaptorPlugin : public Plugin
{
    Q_OBJECT
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    Q_PLUGIN_METADATA(IID "com.nokia.SensorService.Plugin/1.0")
#endif
private:
    void Register(class Loader& l);
};

#endif
