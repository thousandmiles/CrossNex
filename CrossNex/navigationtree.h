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

private slots:
    void showContextMenu(const QPoint &pos);
    void createFolder();
    void createInstance();

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

    bool isParentFolderNameUsed;

private:
    void addFolder(const QString &folderName);
    void addInstance(const QString &instanceName);
    void deleteCurrentNode();
    void renameNode();

};

#endif // NAVIGATIONTREE_H
