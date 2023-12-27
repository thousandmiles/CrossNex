#include "browserwindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QTimer>

BrowserWindow::BrowserWindow(QWidget *parent)
    : QWidget{parent},
    tabContainer(new TabContainer(this)),
    layout(new QVBoxLayout(this))
{
    layout->addWidget(tabContainer);
    setLayout(layout);
    tabContainer->addTab("Home Page", "qrc:/hello/EChart/hello.html");

    //connect(newTabButton, &QPushButton::clicked, this, &BrowserWindow::createNewTab);
}

void BrowserWindow::createNewTab(const QString& nodeName)
{
    tabContainer->addTab(nodeName, "qrc:/echart/EChart/line-simple.html");
}
