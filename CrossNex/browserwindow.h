#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include <QObject>
#include <QWidget>
#include "tabcontainer.h"

class BrowserWindow : public QWidget
{
    Q_OBJECT
public:
    explicit BrowserWindow(QWidget *parent = nullptr);

private:
    TabContainer *tabContainer;

private slots:
    void createNewTab();

};

// ---------------------------------------
// BrowserWindow
// ---------------------------------------

#endif // BROWSERWINDOW_H
