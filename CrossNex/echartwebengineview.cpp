#include "echartwebengineview.h"
#include <QFile>

EchartWebEngineView::EchartWebEngineView(QWidget *parent):
    QWebEngineView(parent)
{
    this->load(QUrl("qrc:/echart/EChart/line-simple.html"));

}
