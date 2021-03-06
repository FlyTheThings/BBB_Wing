#ifndef FLIGHT_NAVIGATOR_NODE_H
#define FLIGHT_NAVIGATOR_NODE_H

#include "abstract_node_frequency.h"

namespace domain
{
    class AbstractNavigationState;

    class FlightNavigatorNode: public AbstractNodeFrequency
    {
        Q_OBJECT

    public:
        FlightNavigatorNode(QObject* parent = nullptr);
        ~FlightNavigatorNode() override;

    public slots:
        void init() override;
        void exec() override;

    private slots:
        void onStateRequested(AbstractNavigationState* state);

    private:
        class Impl;
        Impl* const d;
    };
}

#endif // FLIGHT_NAVIGATOR_NODE_H
