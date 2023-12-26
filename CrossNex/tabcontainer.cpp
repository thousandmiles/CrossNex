#include "tabcontainer.h"

TabContainer::TabContainer(QWidget *parent) : QTabWidget(parent) {
    setTabsClosable(true);

    connect(this, &QTabWidget::tabCloseRequested, [=](int index) {
        removeTab(index);
    });
}
