#include "mainwindow.h"

#include <QApplication>
#include <QtQuick/QQuickView>

#include <QGuiApplication>
#include <QQmlApplicationEngine>


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow w;

    // w.setGeometry(100, 100, windowWidth, windowHeight);
    // w.setFixedSize(windowWidth, windowHeight);

    w.show();

    // QQmlApplicationEngine engine;
    // engine.load(QUrl(QStringLiteral("qrc:/framework.qml")));  // 加载你的QML文件，这里假设文件名为main.qml

    // if (engine.rootObjects().isEmpty())
    //     return -1;

    return app.exec();
}
