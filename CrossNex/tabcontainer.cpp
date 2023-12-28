#include "tabcontainer.h"
#include "tab.h"

TabContainer::TabContainer(QWidget *parent) : QTabWidget(parent)
{
    setTabsClosable(true);

    this->addTab("Home Page", "qrc:/hello/EChart/hello.html");

    connect(this, &QTabWidget::tabCloseRequested, [=](int index) {
        removeTab(index);
    });
}

void TabContainer::addTab(const QString &nodeName, const QString &url) {
    Tab *tab = new Tab(url, this);
    int index = QTabWidget::addTab(tab, nodeName);
    setCurrentIndex(index);
}

void TabContainer::createNewTab(const QString& nodeName)
{
    this->addTab(nodeName, "qrc:/echart/EChart/line-simple.html");
}
