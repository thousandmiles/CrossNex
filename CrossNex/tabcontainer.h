#ifndef TABCONTAINER_H
#define TABCONTAINER_H

#include <QObject>
#include <QTabWidget>

class TabContainer : public QTabWidget
{
    Q_OBJECT
public:
    TabContainer(QWidget *parent = nullptr);

    void addTab(const QString& nodeName, const QString& url);
public slots:
    void createNewTab(const QString &nodeName, const QString& ipAddress);
};

#endif // TABCONTAINER_H
