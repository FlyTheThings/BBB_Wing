#ifndef SENSOR_ALTIMETER_NODE_H
#define SENSOR_ALTIMETER_NODE_H

#include "abstract_node_frequency.h"

namespace domain
{
    class SensorAltimeterNode: public AbstractNodeFrequency
    {
        Q_OBJECT

    public:
        SensorAltimeterNode(float frequency, QObject* parent = nullptr);
        ~SensorAltimeterNode() override;

        void init() override;
        void exec() override;

    private:
        class Impl;
        Impl* const d;
    };
}

#endif // SENSOR_ALTIMETER_NODE_H
