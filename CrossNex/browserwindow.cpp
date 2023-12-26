#include "browserwindow.h"
#include <QVBoxLayout>
#include <QPushButton>

BrowserWindow::BrowserWindow(QWidget *parent)
    : QWidget{parent}
{
    tabContainer = new TabContainer(this);
    QPushButton *newTabButton = new QPushButton("New Tab", this);

    connect(newTabButton, &QPushButton::clicked, this, &BrowserWindow::createNewTab);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(newTabButton);
    layout->addWidget(tabContainer);
    setLayout(layout);
}

void BrowserWindow::createNewTab() {
    tabContainer->addTab("qrc:/echart/EChart/line-simple.html");
}
