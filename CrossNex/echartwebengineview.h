#ifndef ECHARTWEBENGINEVIEW_H
#define ECHARTWEBENGINEVIEW_H

#include <QObject>
#include <QWebEngineView>

class EchartWebEngineView : public QWebEngineView
{
    Q_OBJECT
public:
    EchartWebEngineView(QWidget *parent = nullptr);
};

#endif // ECHARTWEBENGINEVIEW_H
