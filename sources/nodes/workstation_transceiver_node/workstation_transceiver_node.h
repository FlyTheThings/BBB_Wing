#ifndef WORKSTATION_TRANSCEIVER_NODE_H
#define WORKSTATION_TRANSCEIVER_NODE_H

#include "branch_node.h"

namespace domain
{
    class WorkstationTransceiverNode: public BranchNode
    {
        Q_OBJECT

    public:
        WorkstationTransceiverNode(QObject* parent = nullptr);
        ~WorkstationTransceiverNode() override;

    public slots:
        void init() override;

    private slots:
        void onPacketReceived(const QByteArray& packet);

        void setActiveWireLine();
        void setActiveAirLine();
        void setInactiveLine();

    private:
        class Impl;
        Impl* const d;
    };
}

#endif // WORKSTATION_TRANSCEIVER_NODE_H
