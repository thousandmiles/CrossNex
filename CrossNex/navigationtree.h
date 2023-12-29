#ifndef NAVIGATIONTREE_H
#define NAVIGATIONTREE_H

#include <QObject>
#include <QTreeWidget>
#include <QWidget>
#include <QMenu>
#include <QMouseEvent>

class NavigationTree : public QTreeWidget
{
    Q_OBJECT
public:
    NavigationTree(QWidget *parent = nullptr);
    void dropEvent(QDropEvent *event) override;

    void traverseTree(QTreeWidgetItem *item);
    void setTreeSet(QTreeWidgetItem *item);
    void setCurrentLevelSet(QTreeWidgetItem *item);
    void setNextLevelSet(QTreeWidgetItem *item);
    bool isDeleteFolder();
    bool isDeleteInstance();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    QString getNodePath(QTreeWidgetItem *node);


private slots:
    void showContextMenu(const QPoint &pos);
    void createFolder();
    void createInstance();
    void handleNodeClicked(QTreeWidgetItem *item, int column);

private:
    QScopedPointer<QTreeWidgetItem> rootNode;
    QTreeWidgetItem *currentNode;
    QMenu contextMenu;
    QSet<QString> instanceSet;
    QSet<QString> folderSet;
    QSet<QString> currentLevelInstanceSet;
    QSet<QString> currentLevelFolderSet;
    QSet<QString> nextLevelInstanceSet;
    QSet<QString> nextLevelFolderSet;

    QList<QTreeWidgetItem *>currentLevelInstanceList;
    QList<QTreeWidgetItem *>currentLevelFolderList;

    QHash<QString, QString> name_ip;    // 维护所有的实例名及对应IP地址，【新建/重命名/删除】实例时要更新它

    bool isParentFolderNameUsed;

private:
    void addFolder(const QString &folderName);
    void addInstance(const QString &instanceName, const QString &ipAddress);
    void deleteCurrentNode();
    void renameNode();


signals:
    void nodeClicked(const QString& nodeName, const QString&ipAddress);
};

#endif // NAVIGATIONTREE_H
