#include "gui_node.h"

// Qt
#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickView>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlContext>
#include <QDebug>

// Internal
#include "core.h"

#include "subscriber.h"
#include "publisher.h"

#include "config_service.h"
#include "board_service.h"
#include "ground_service.h"
#include "video_service.h"

inline void initResources()
{
    Q_INIT_RESOURCE(qml);
    Q_INIT_RESOURCE(resources);
}

using namespace domain;

class GuiNode::Impl
{
public:
    QQuickView view;
    Subscriber sub;
    Publisher pub;

    ConfigService configService;
    BoardService boardService;
    GroundService groundService;
    VideoService videoService;
};

GuiNode::GuiNode(QObject* parent):
    AbstractNode(parent),
    d(new Impl())
{
    initResources();

    d->pub.bind(endpoints::gui);

    connect(&d->boardService, &BoardService::publish,
            &d->pub, static_cast<void (Publisher::*)
            (const QString&, const QByteArray&)>(&Publisher::publish));

    d->view.rootContext()->setContextProperty("boardService", &d->boardService);
    d->view.rootContext()->setContextProperty("groundService", &d->groundService);
    d->view.rootContext()->setContextProperty("videoService", &d->videoService);
    d->view.rootContext()->setContextProperty("configService", &d->configService);

    d->view.setSource(QUrl(QStringLiteral("qrc:/qml/Views/MainView.qml")));
    d->view.setResizeMode(QQuickView::SizeRootObjectToView);

    QObject::connect(d->view.engine(), &QQmlEngine::quit,
                     qApp, &QGuiApplication::quit);
}

GuiNode::~GuiNode()
{
    delete d;
}

void GuiNode::init()
{
    d->sub.connectTo( { endpoints::groundTransceiver, endpoints::player } );
    d->sub.subscribe(topics::all);
    connect(&d->sub, &Subscriber::received, this, &GuiNode::onSubReceived);
}

void GuiNode::start()
{
    d->view.showFullScreen();
}

void GuiNode::onSubReceived(const QString& topic, const QByteArray& data)
{
    if (topic == topics::altPacket)
        d->boardService.updateAltData(AltPacket::fromByteArray(data));
    else if (topic == topics::snsPacket)
        d->boardService.updateSnsData(SnsPacket::fromByteArray(data));
    else if (topic == topics::insPacket)
        d->boardService.updateInsData(InsPacket::fromByteArray(data));
    else if (topic == topics::connectionStatusPacket)
        d->groundService.updateConnectionStatus(
                    ConnectionStatusPacket::fromByteArray(data));
}
