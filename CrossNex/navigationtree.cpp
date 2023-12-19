#include "navigationtree.h"
#include "navigationtree.moc"
#include "newinstancedialog.h"
#include <QInputDialog>

NavigationTree::NavigationTree(QWidget *parent):
    QTreeWidget(parent),
    contextMenu(this),
    rootNode(new QTreeWidgetItem(this)),
    currentNode(nullptr)
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
    currentNode = itemAt(pos);
    contextMenu.clear();

    if (!currentNode) return;

    QAction *addAction = nullptr;
    QAction *deleteAction = nullptr;
    QAction *renamFolderAction = nullptr;
    QAction *addFolderAction = nullptr;
    QAction *deleteFolderAction = nullptr;
    QAction *renameNodeAction = nullptr;

    if (currentNode == rootNode.data())
    {
        addAction = contextMenu.addAction(QIcon(new_instance), "新建实例");
        addFolderAction = contextMenu.addAction(QIcon(new_folder), "新建文件夹");

        connect(addFolderAction, &QAction::triggered, this, &NavigationTree::createFolder);
        connect(addAction, &QAction::triggered, this, &NavigationTree::createInstance);

        QPoint pos = QCursor::pos();
        contextMenu.exec(pos);
    }
    else
    {
        if (currentNode->type() == CustomTreeWidgetItem::FolderType)
        {
            addAction = contextMenu.addAction(QIcon(new_instance), "新建实例");
            renamFolderAction = contextMenu.addAction(QIcon(rename_folder), "重命名文件夹");
            addFolderAction = contextMenu.addAction(QIcon(new_folder), "新建文件夹");
            deleteFolderAction = contextMenu.addAction(QIcon(delete_folder), "删除文件夹");

            connect(addFolderAction, &QAction::triggered, this, &NavigationTree::createFolder);
            connect(addAction, &QAction::triggered, this, &NavigationTree::createInstance);
        }
        else
        {
            renameNodeAction = contextMenu.addAction(QIcon(rename_instance), "重命名实例");
            deleteAction = contextMenu.addAction(QIcon(delete_instance), "删除实例");
        }

        QAction *selectedAction = contextMenu.exec(mapToGlobal(pos));

        if (selectedAction == addAction)
        {
            // do something ?
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
        addFolder(folderName);
    }
}

void NavigationTree::createInstance()
{
    NewInstanceDialog newInstanceDlg;
    if (newInstanceDlg.exec() == QDialog::Accepted)
    {
        // 用户点击了OK按钮
        QString ipAddress = newInstanceDlg.getIpAddress();
        QString instanceName = newInstanceDlg.getInstanceName();

        // 在这里可以使用获取到的IP地址和实例名进行后续处理
        qDebug() << "IP Address: " << ipAddress;
        qDebug() << "Instance Name: " << instanceName;

        addInstance(instanceName);
    }
}

void NavigationTree::addFolder(const QString &folderName)
{
    CustomTreeWidgetItem *folderItem = new CustomTreeWidgetItem(currentNode, CustomTreeWidgetItem::FolderType);
    folderItem->setText(0, folderName);
    folderItem->setIcon(0, QIcon(folder));
}

void NavigationTree::addInstance(const QString &instanceName)
{
    CustomTreeWidgetItem *instanceItem = new CustomTreeWidgetItem(currentNode, CustomTreeWidgetItem::NodeType);
    instanceItem->setText(0, instanceName);
    instanceItem->setIcon(0, QIcon(instance));
}


