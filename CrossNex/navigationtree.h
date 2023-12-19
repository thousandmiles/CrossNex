#ifndef NAVIGATIONTREE_H
#define NAVIGATIONTREE_H

#include <QObject>
#include <QTreeWidget>
#include <QWidget>
#include <QMenu>

#include "customtreewidgetitem.h"

constexpr const char* delete_folder = ":/icon/plus/resource/delete_folder.png";
constexpr const char* delete_instance = ":/icon/plus/resource/delete_instance.png";
constexpr const char* new_root = ":/icon/plus/resource/new.png";
constexpr const char* new_folder = ":/icon/plus/resource/new_folder.png";
constexpr const char* new_instance = ":/icon/plus/resource/new_instance.png";
constexpr const char* rename_folder = ":/icon/plus/resource/rename.png";
constexpr const char* rename_instance = ":/icon/plus/resource/rename.png";
constexpr const char* folder = ":/icon/plus/resource/folder.png";
constexpr const char* instance = ":/icon/plus/resource/instance.png";

class NavigationTree : public QTreeWidget
{
    Q_OBJECT
public:
    NavigationTree(QWidget *parent = nullptr);
    void dropEvent(QDropEvent *event) override;

    void traverseTree(QTreeWidgetItem *item);
    void clearTreeSet();
    bool isDeleteFolder();

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

private:
    void addFolder(const QString &folderName);
    void addInstance(const QString &instanceName);
    void deleteCurrentNode();

};

#endif // NAVIGATIONTREE_H
