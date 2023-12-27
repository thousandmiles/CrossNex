#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "tabcontainer.h"

class BrowserWindow : public QWidget
{
    Q_OBJECT
public:
    explicit BrowserWindow(QWidget *parent = nullptr);

private:
    TabContainer *tabContainer;
    QPushButton *newTabButton;
    QPushButton *homeTabButton;
    QVBoxLayout *layout;

public slots:
    void createNewTab(const QString& nodeName);

};

// ---------------------------------------
// BrowserWindow
// ---------------------------------------

#endif // BROWSERWINDOW_H
