#ifndef TABCONTAINER_H
#define TABCONTAINER_H

#include <QObject>
#include <QTabWidget>
#include "tab.h"

class TabContainer : public QTabWidget
{
    Q_OBJECT
public:
    TabContainer(QWidget *parent = nullptr);

    void addTab(const QString& nodeName, const QString& url) {
        Tab *tab = new Tab(url, this);
        int index = QTabWidget::addTab(tab, nodeName);
        setCurrentIndex(index);
    }
};

#endif // TABCONTAINER_H
