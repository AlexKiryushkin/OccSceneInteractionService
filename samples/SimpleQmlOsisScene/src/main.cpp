
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQuick/QQuickView>

#include <QmlOsis/QmlOsisView.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qputenv("QSG_RENDER_LOOP", "threaded");
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);

    qmlRegisterType<osis::qml::QmlOsisView>("OpenCascade", 7, 3, "QmlOsisView");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
