#include "tab.h"
#include <QVBoxLayout>

Tab::Tab(const QString& url, QWidget *parent)
    : QWidget{parent}
{
    webEngineView = new QWebEngineView(this);
    webEngineView->load(QUrl(url));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(webEngineView);
    setLayout(layout);
}
