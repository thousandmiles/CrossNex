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
    bool isValidInstanceName(const QString &instanceName);

    void setInstanceSet(const QSet<QString>& itemSet);


private slots:
    void acceptClicked();

private:
    QLineEdit *ipLineEdit;
    QLineEdit *instanceLineEdit;

    QSet<QString> instanceSet;

    static const QRegularExpression ipv4Regex;
    QSet<QString> namelimits;
};

#endif // NEWINSTANCEDIALOG_H
