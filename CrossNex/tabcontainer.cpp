#include "tabcontainer.h"
#include "tab.h"

TabContainer::TabContainer(QWidget *parent) : QTabWidget(parent)
{
    setTabsClosable(true);

    addHelloPage();

    connect(this, &QTabWidget::tabCloseRequested, [=](int index) {
        removeTab(index);
    });
}

void TabContainer::addTab(const QString &nodeName, const QString &ipAddress) {
    Tab *tab = new Tab(ipAddress, this);
    int index = QTabWidget::addTab(tab, nodeName);
    setCurrentIndex(index);
}

void TabContainer::addHelloPage()
{
    Hello * hellopage = new Hello(this);
    int index = QTabWidget::addTab(hellopage, "Home Page");
    setCurrentIndex(index);
}

void TabContainer::createNewTab(const QString& nodeName, const QString& ipAddress)
{
    this->addTab(nodeName, ipAddress);
}
