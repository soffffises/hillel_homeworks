#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QDir>

#include "TemperatureModel.h"
#include "TemperatureController.h"

int main(int argc, char *argv[]) {

    QQuickStyle::setStyle("Fusion");
    QQuickStyle::setFallbackStyle("Basic");

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    TemperatureModel model;
    TemperatureController controller(&model);

    engine.rootContext()->setContextProperty("temperatureModel", &model);
    engine.rootContext()->setContextProperty("temperatureController", &controller);

    QString qmlPath = QDir(QCoreApplication::applicationDirPath()).filePath("Main.qml");

    engine.load(QUrl::fromLocalFile(qmlPath));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
