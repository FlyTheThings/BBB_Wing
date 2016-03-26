#include "board_gateway_node.h"

// Qt
#include <QTimer>
#include <QMap>
#include <QDebug>

// Internal
#include "config.h"
#include "topics.h"

#include "subscriber.h"
#include "publisher.h"

#include "udp_exchanger.h"
#include "serial_port_exchanger.h"

#include "transmission_packet.h"

using namespace domain;

class BoardGatewayNode::Impl
{
public:
    Subscriber sub;
    Publisher pub;

    AbstractExchanger* wireLine;
    AbstractExchanger* airLine;

    QTimer* timoutTimer;

    bool wireReceived = false;
    bool airReceived = false;
    QMap <QString, QByteArray> dataMap;
};

BoardGatewayNode::BoardGatewayNode(QObject* parent):
    AbstractNodeFrequency(0, parent),
    d(new Impl())
{
    Config::begin("BoardGateway");
    d->pub.bind("ipc://board_gateway");

    this->setFrequency(Config::setting("frequency").toFloat());

    d->wireLine = new UdpExchanger(
                      Config::setting("udp_board_port").toInt(),
                      QHostAddress(Config::setting("udp_ground_address").toString()),
                      Config::setting("udp_ground_port").toInt(), this);
    connect(d->wireLine, &AbstractExchanger::received,
            this, &BoardGatewayNode::onLineReceived);

    d->airLine = new SerialPortExchanger(
                     Config::setting("serial_port_board").toString(), this);
    connect(d->airLine, &AbstractExchanger::received,
            this, &BoardGatewayNode::onLineReceived);

    d->timoutTimer = new QTimer(this);
    d->timoutTimer->setInterval(Config::setting("timeout_interval").toInt());
    d->timoutTimer->setSingleShot(true);
    connect(d->timoutTimer, &QTimer::timeout, this, &BoardGatewayNode::onTimeout);

    Config::end();
}

BoardGatewayNode::~BoardGatewayNode()
{
    delete d;
}

void BoardGatewayNode::init()
{
     d->sub.connectTo("ipc://alt");
     d->sub.connectTo("ipc://ins");
     d->sub.connectTo("ipc://sns");
     d->sub.connectTo("ipc://navigator");
     d->sub.connectTo("ipc://controller");

     d->sub.subscribe("");
}

void BoardGatewayNode::start()
{
    AbstractNodeFrequency::start();

    d->wireLine->start();
    d->airLine->start();

    connect(&d->sub, &Subscriber::received, this,
            &BoardGatewayNode::onSubReceived);
}

void BoardGatewayNode::exec()
{
    for (const QString& topic: d->dataMap.keys())
    {
        TransmissionPacket packet;

        packet.topic = topic;
        packet.data = d->dataMap[topic];
        packet.calcCrc();

        if (d->wireReceived) d->wireLine->transmit(packet.toByteArray());
        if (d->airReceived) d->airLine->transmit(packet.toByteArray());
    }

    d->dataMap.clear();
}

void BoardGatewayNode::onTimeout()
{
    d->wireReceived = false;
    d->airReceived = false;
}

void BoardGatewayNode::onSubReceived(const QString& topic,
                                     const QByteArray& data)
{
    d->dataMap[topic] = data;
}

void BoardGatewayNode::onLineReceived(const QByteArray& data)
{
    if (this->sender() == d->wireLine) d->wireReceived = true;
    else if (this->sender() == d->airLine) d->airReceived = true;

    auto packet = TransmissionPacket::fromByteArray(data);
    if (!packet.validateCrc()) return;
    d->pub.publish(packet.topic, packet.data);

    d->timoutTimer->start();
}