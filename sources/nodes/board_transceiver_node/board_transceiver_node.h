#ifndef BOARD_TRANSCEIVER_NODE_H
#define BOARD_TRANSCEIVER_NODE_H

#include "abstract_node_frequency.h"

namespace domain
{
    class BoardTransceiverNode: public AbstractNodeFrequency
    {
        Q_OBJECT

    public:
        BoardTransceiverNode(float frequency, QObject* parent = nullptr);
        ~BoardTransceiverNode() override;

    public slots:
        void init() override;
        void exec() override;

    private slots:
        void onSubReceived(const QString& topic, const QByteArray& msg);

    private:
        class Impl;
        Impl* const d;
    };
}

#endif // BOARD_TRANSCEIVER_NODE_H
