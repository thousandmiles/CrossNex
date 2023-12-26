#ifndef NEWINSTANCEDIALOG_H
#define NEWINSTANCEDIALOG_H

#include <QDialog>
#include <QObject>
#include <QLineEdit>
#include <QRegularExpression>

class NewInstanceDialog : public QDialog
{
    Q_OBJECT

public:
    NewInstanceDialog(QWidget *parent = nullptr);

    QString getIpAddress() const;

    QString getInstanceName() const;

    bool isValidIpAddress(const QString &ipAddress);

    void setInstanceSet(const QSet<QString>& itemSet);


private slots:
    void acceptClicked();

private:
    QLineEdit *ipLineEdit;
    QLineEdit *instanceLineEdit;

    QSet<QString> instanceSet;

    static const QRegularExpression ipv4Regex;
};

#endif // NEWINSTANCEDIALOG_H