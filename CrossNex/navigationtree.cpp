#include "navigationtree.h"
#include "navigationtree.moc"
#include <QInputDialog>

NavigationTree::NavigationTree(QWidget *parent):
    QTreeWidget(parent),
    contextMenu(this),
    rootNode(new CustomTreeWidgetItem(this))
{
    setColumnCount(1);
    setDragEnabled(true);
    setAcceptDrops(true);
    setDefaultDropAction(Qt::MoveAction);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setContextMenuPolicy(Qt::CustomContextMenu);

    this->setHeaderLabel("实例列表");

    rootNode->setText(0, "新建");
    rootNode->setExpanded(true);  // 设置根节点默认展开

    // 添加一个图标，模拟加号形状

    rootNode->setIcon(0, QIcon(new_root));

    connect(this, &NavigationTree::customContextMenuRequested, this, &NavigationTree::showContextMenu);
}

void NavigationTree::dropEvent(QDropEvent *event)
{
    QTreeWidget::dropEvent(event);
    // if (event->source() == this) {
    //     // Handle internal drops
    // } else {
    //     // Handle drops from external sources
    // }
}

void NavigationTree::showContextMenu(const QPoint &pos)
{
    QTreeWidgetItem *item = itemAt(pos);
    contextMenu.clear();

    if (!item) return;

    if (item == rootNode.data())
    {
        QAction *addAction = nullptr;
        QAction *addFolderAction = nullptr;

        addAction = contextMenu.addAction(QIcon(new_instance), "新建实例");
        addFolderAction = contextMenu.addAction(QIcon(new_folder), "新建文件夹");

        connect(addFolderAction, &QAction::triggered, this, &NavigationTree::createFolder);
        QPoint pos = QCursor::pos();
        QAction *selectedAction = contextMenu.exec(pos);

        if (selectedAction == addAction)
        {

        }
        else
        {

        }

    }
    else
    {

        QAction *addAction = contextMenu.addAction(QIcon(new_instance), "新建实例");
        QAction *deleteAction = contextMenu.addAction(QIcon(delete_instance), "删除实例");
        QAction *renamFolderAction = nullptr;
        QAction *addFolderAction = nullptr;
        QAction *deleteFolderAction = nullptr;
        QAction *renameNodeAction = nullptr;

        contextMenu.addSeparator();
        if (item->type() == CustomTreeWidgetItem::FolderType) {
            renamFolderAction = contextMenu.addAction(QIcon(rename_folder), "重命名文件夹");
            addFolderAction = contextMenu.addAction(QIcon(new_folder), "新建文件夹");
            deleteFolderAction = contextMenu.addAction(QIcon(delete_folder), "删除文件夹");
        } else {
            renameNodeAction = contextMenu.addAction(QIcon(rename_instance), "重命名实例");
        }

        QAction *selectedAction = contextMenu.exec(mapToGlobal(pos));

        if (selectedAction == addAction)
        {

        }
        else if (selectedAction == deleteAction)
        {

        }
        else if (selectedAction == renamFolderAction)
        {

        }
        else if (selectedAction == addFolderAction)
        {

        }
        else if (selectedAction == renameNodeAction)
        {

        }
        else if (selectedAction == deleteFolderAction)
        {

        }
    }
}

void NavigationTree::createFolder()
{
    bool ok;
    QString folderName = QInputDialog::getText(this, "新建文件夹", "文件夹名称:", QLineEdit::Normal, "", &ok);

        if (ok && !folderName.isEmpty()) {
            // 添加新的文件夹节点
            addFolder(folderName);
        }
}

void NavigationTree::addFolder(const QString &folderName)
{
    CustomTreeWidgetItem *folderItem = new CustomTreeWidgetItem(this, CustomTreeWidgetItem::FolderType);
    folderItem->setText(0, folderName);
    folderItem->setIcon(0, QIcon(folder));
}


