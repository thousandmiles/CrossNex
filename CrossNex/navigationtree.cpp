#include "navigationtree.h"
#include "navigationtree.moc"
#include "newinstancedialog.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QTreeWidgetItemIterator>
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
//constexpr const char* expanded = ":/icon/plus/resource/expanded_triangle.png";
//constexpr const char* collapsed = ":/icon/plus/resource/collapsed_triangle.png";

NavigationTree::NavigationTree(QWidget *parent):
    QTreeWidget(parent),
    rootNode(new QTreeWidgetItem(this)),
    currentNode(nullptr),
    contextMenu(this)
{
    setColumnCount(1);
    //setDragEnabled(true);
    //setAcceptDrops(true);
    setDefaultDropAction(Qt::MoveAction);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setContextMenuPolicy(Qt::CustomContextMenu);

    rootNode->setText(0, "新建");
    rootNode->setExpanded(true);  // 设置根节点默认展开
    rootNode->setIcon(0, QIcon(new_root));

    isParentFolderNameUsed = false;

    connect(this, &NavigationTree::itemClicked, this, &NavigationTree::handleNodeClicked);
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

void NavigationTree::traverseTree(QTreeWidgetItem *item) {
    if (!item) {
        return;
    }

    if (item->type() == CustomTreeWidgetItem::FolderType)
    {
        folderSet<<item->text(0);
        for (int i = 0; i < item->childCount(); ++i) {
            traverseTree(item->child(i));
        }
    }
    else
    {
        instanceSet<<item->text(0);
        for (int i = 0; i < item->childCount(); ++i) {
            traverseTree(item->child(i));
        }
    }


}

/* 获取当前节点下的所有子节点，包含文件夹和实例 */
void NavigationTree::setTreeSet(QTreeWidgetItem *item)
{
    folderSet.clear();
    instanceSet.clear();
    traverseTree(item);
}

/* 获取当前节点的所有同级子节点，不搜索同级节点的子节点 */
void NavigationTree::setCurrentLevelSet(QTreeWidgetItem *item)
{

    if (!item->parent())
    {
        return;
    }

    currentLevelInstanceSet.clear();
    currentLevelFolderSet.clear();

    QTreeWidgetItem* parentNode = item->parent();
    QTreeWidgetItemIterator it(item->parent());
    while ((*it))
    {
        if ((*it)->parent() && (*it)->parent() == parentNode)
        {
            QString currentName = (*it)->text(0);

            if((*it)->type() == CustomTreeWidgetItem::NodeType)
            {
                currentLevelInstanceSet<<currentName;
                currentLevelInstanceList<<(*it);
            }
            else
            {
                currentLevelFolderSet<<currentName;
                currentLevelFolderList<<(*it);
            }
        }
        ++it;
    }
}

/* 获取当前节点的下一级所有的同级子节点，不搜索同级节点的子节点 */
void NavigationTree::setNextLevelSet(QTreeWidgetItem *item)
{
    nextLevelInstanceSet.clear();
    nextLevelFolderSet.clear();

    QTreeWidgetItem* currNode = item;
    QTreeWidgetItemIterator it(item);
    while ((*it))
    {
        if ((*it)->parent() && (*it)->parent() == currNode)
        {
            QString currentName = (*it)->text(0);

            if((*it)->type() == CustomTreeWidgetItem::NodeType)
            {
                nextLevelInstanceSet<<currentName;
            }
            else
            {
                nextLevelFolderSet<<currentName;
            }
        }
        ++it;
    }
}

bool NavigationTree::isDeleteFolder()
{
    QMessageBox msgBox;
    msgBox.setText("删除文件夹会删除当前文件夹下的所有实例，\n是否继续删除？");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    return msgBox.exec() == QMessageBox::Yes ? true : false;
}

bool NavigationTree::isDeleteInstance()
{
    QMessageBox msgBox;
    msgBox.setText("是否要删除当前实例？");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    return msgBox.exec() == QMessageBox::Yes ? true : false;
}

void NavigationTree::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        QTreeWidgetItem *item = itemAt(event->pos());
        currentNode = item;
        if (item)
        {
            // 打开图表区
        }
    }

    // 调用基类的事件处理函数
    QTreeWidget::mousePressEvent(event);
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

    // setTreeSet(currentNode);
    setCurrentLevelSet(currentNode);
    setNextLevelSet(currentNode);

    if (currentNode == rootNode.data())
    {
        addAction = contextMenu.addAction(QIcon(new_instance), "新建实例");
        addFolderAction = contextMenu.addAction(QIcon(new_folder), "新建文件夹");

        connect(addFolderAction, &QAction::triggered, this, &NavigationTree::createFolder);
        connect(addAction, &QAction::triggered, this, &NavigationTree::createInstance);
        contextMenu.exec(mapToGlobal(pos));
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


        if (addAction && selectedAction == addAction)
        {
            // do something ?
        }
        else if (deleteAction && selectedAction == deleteAction)
        {
            bool del = isDeleteInstance();
            if (del)
            {
                deleteCurrentNode();
            }
        }
        else if (renamFolderAction && selectedAction == renamFolderAction)
        {
            renameNode();
        }
        else if (addFolderAction && selectedAction == addFolderAction)
        {

        }
        else if (renameNodeAction && selectedAction == renameNodeAction)
        {
            renameNode();
        }
        else if (deleteFolderAction && selectedAction == deleteFolderAction)
        {
            bool del = isDeleteFolder();
            if (del)
            {
                deleteCurrentNode();
            }
        }
    }
}

void NavigationTree::createFolder()
{

    setTreeSet(rootNode.data());

    bool ok = false;
    QString folderName;

    while (!ok) {
        folderName = QInputDialog::getText(nullptr, "输入", "请输入文件夹名:", QLineEdit::Normal, "", &ok);
        if (!ok)
        {
            break;
        }

        if (folderSet.contains(folderName) || folderName == currentNode->text(0))
        {
            QMessageBox::warning(nullptr, "Error", "文件夹已存在，请重新输入。");
            ok = false;  // 设置标志为false，继续循环
            continue;
        }
    }

    if (ok && !folderName.isEmpty()) {
        addFolder(folderName);
    }
}

void NavigationTree::createInstance()
{
    NewInstanceDialog newInstanceDlg;

    setTreeSet(rootNode.data());

    newInstanceDlg.setInstanceSet(instanceSet);

    if (newInstanceDlg.exec() == QDialog::Accepted)
    {
        // 用户点击了OK按钮
        QString ipAddress = newInstanceDlg.getIpAddress();
        QString instanceName = newInstanceDlg.getInstanceName();

        name_ip.insert(instanceName, ipAddress);

        addInstance(instanceName, ipAddress);
    }
}

void NavigationTree::handleNodeClicked(QTreeWidgetItem *item, int column)
{
    if (item != rootNode.data() &&
        item->type() == CustomTreeWidgetItem::NodeType)
    {
        QString nodeName = item->text(column);

        if (!name_ip.contains(nodeName))
        {
            QMessageBox::warning(nullptr, "Error", "当前实例未配置IP地址！");
        }

        emit nodeClicked(nodeName, name_ip[nodeName]);
    }
}

void NavigationTree::addFolder(const QString &folderName)
{
    CustomTreeWidgetItem *folderItem = new CustomTreeWidgetItem(currentNode, CustomTreeWidgetItem::FolderType);
    folderItem->setText(0, folderName);
    folderItem->setIcon(0, QIcon(folder));

    this->sortItems(0, Qt::AscendingOrder);
}

void NavigationTree::addInstance(const QString &instanceName, const QString &ipAddress)
{
    CustomTreeWidgetItem *instanceItem = new CustomTreeWidgetItem(currentNode, CustomTreeWidgetItem::NodeType);
    instanceItem->setText(0, instanceName);
    instanceItem->setIcon(0, QIcon(instance));
    instanceItem->ipAddress = ipAddress;

    this->sortItems(0, Qt::AscendingOrder);
}

void NavigationTree::deleteCurrentNode()
{
    QTreeWidgetItem *parentItem = currentNode->parent();
    if (parentItem) {
        int relativeIndex = parentItem->indexOfChild(currentNode);
        parentItem->takeChild(relativeIndex);
        delete currentNode;
    }
}

void NavigationTree::renameNode()
{
    bool ok = false;
    QString newName;

    // setCurrentLevelSet(currentNode);
    setTreeSet(rootNode.data());

    while (!ok) {
        newName = QInputDialog::getText(nullptr, "重命名", "请输入新的名字:", QLineEdit::Normal, "", &ok);
        if (!ok)
        {
            break;
        }

        if (newName == currentNode->text(0))
        {
            QMessageBox::warning(nullptr, "Error", "名字已存在，请重新输入。");
            ok = false;  // 设置标志为false，继续循环
            continue;
        }

        if (currentNode->type() == CustomTreeWidgetItem::NodeType && instanceSet.contains(newName))
        {
            QMessageBox::warning(nullptr, "Error", "名字已存在，请重新输入。");
            ok = false;  // 设置标志为false，继续循环
            continue;
        }

        if (currentNode->type() == CustomTreeWidgetItem::FolderType && folderSet.contains(newName))
        {
            QMessageBox::warning(nullptr, "Error", "名字已存在，请重新输入。");
            ok = false;  // 设置标志为false，继续循环
            continue;
        }

    }

    if (ok && !newName.isEmpty()) {
        currentNode->setText(0, newName);
    }
}


