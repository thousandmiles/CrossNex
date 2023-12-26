#ifndef TAB_H
#define TAB_H

#include <QWidget>
#include <QWebEngineView>

class Tab : public QWidget
{
    Q_OBJECT
public:
    explicit Tab(const QString& url, QWidget *parent = nullptr);

signals:

private:
    QWebEngineView *webEngineView;
};

#endif // TAB_H
