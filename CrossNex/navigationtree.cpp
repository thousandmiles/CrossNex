#include "navigationtree.h"
#include "navigationtree.moc"
#include "newinstancedialog.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QTreeWidgetItemIterator>
#include <QStandardPaths>
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

    // 构建文件路径
    configFilePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/CrossNex";
    instanceConfigFile = configFilePath + "/CrossNexII.cnx";
    folderConfigFile = configFilePath + "/CrossNexFF.cnx";

    createConfigFile();

    connect(this, &NavigationTree::itemClicked, this, &NavigationTree::handleNodeClicked);
    connect(this, &NavigationTree::customContextMenuRequested, this, &NavigationTree::showContextMenu);
    connect(this, &NavigationTree::instanceNameChanged, this, &NavigationTree::handleInstanceNameChanged);
    connect(this, &NavigationTree::instanceCreated, this, &NavigationTree::handleInstanceCreated);
    connect(this, &NavigationTree::folderNameChanged, this, &NavigationTree::handleFolderNameChanged);
    connect(this, &NavigationTree::folderCreated, this, &NavigationTree::handleFolderCreated);
    connect(this, &NavigationTree::instanceDeleted, this, &NavigationTree::handleInstanceDeleted);
    connect(this, &NavigationTree::folderDeleted, this, &NavigationTree::handleFolderDeleted);

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
    if (!item)
    {
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

bool NavigationTree::isFileExist(const QString& filePath)
{
    QFile file(filePath);
    return file.exists();
}

bool NavigationTree::isDirExist(const QString& dirPath)
{
    QDir dir(dirPath);
    return dir.exists();
}

// 此函数功能可由handleNodeClicked(...)代替
void NavigationTree::mousePressEvent(QMouseEvent *event)
{

    if (event->button() == Qt::LeftButton)
    {
        QTreeWidgetItem *item = itemAt(event->pos());
        currentNode = item;

        if (item)
        {
            qDebug()<<"mouse pressed: "<<currentNode->text(0);

            /*
            if (item != rootNode.data() &&
                item->type() == CustomTreeWidgetItem::NodeType)
            {
                QString nodeName = item->text(0);

                if (!name_ip.contains(nodeName))
                {
                    QMessageBox::warning(nullptr, "Error", "当前实例未配置IP地址！");
                }

                emit nodeClicked(nodeName, name_ip[nodeName]);
            }
            */
        }
    }

    // 调用基类的事件处理函数
    QTreeWidget::mousePressEvent(event);
}

QString NavigationTree::getNodePath(QTreeWidgetItem *node)
{
    QString path;
    QTreeWidgetItem * it = node;

    if (it->type() == CustomTreeWidgetItem::FolderType)
    {
        path += "/";
    }

    while (it)
    {
        QString nodeName = it->text(0);
        path.prepend("/" + nodeName);
        it = it->parent();
    }

    return path;
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

    while (!ok)
    {
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
        addInstance(instanceName, ipAddress);
    }
}

void NavigationTree::handleNodeClicked(QTreeWidgetItem *item, int column)
{
    if (item != rootNode.data() &&
        item->type() == CustomTreeWidgetItem::NodeType)
    {
        QString nodeName = item->text(column);

        if (!instanceName_ip.contains(nodeName))
        {
            QMessageBox::warning(nullptr, "Error", "当前实例未配置IP地址！");
        }

        emit nodeClicked(nodeName, instanceName_ip[nodeName]);
    }
}

void NavigationTree::handleInstanceNameChanged(const QString &previous, const QString &current)
{
    instanceName_ip.insert(current, instanceName_ip[previous]);
    instanceName_ip.remove(previous);

    instanceName_path.insert(current, instanceName_path[previous]);
    instanceName_path.remove(previous);

    instanceName_date.insert(current, instanceName_date[previous]);
    instanceName_date.remove(previous);
}

void NavigationTree::handleInstanceCreated(const Instance_Node_Config &config, const QString &instanceName)
{
    instanceName_ip.insert(instanceName, config.IP);
    instanceName_path.insert(instanceName, config.Path);
    instanceName_date.insert(instanceName, config.Date);
}

void NavigationTree::handleFolderNameChanged(const QString &previous, const QString &current)
{
    folderName_path.insert(current, folderName_path[previous]);
    folderName_path.remove(previous);
}

void NavigationTree::handleFolderCreated(const Folder_Node_Config &config, const QString &folderName)
{
    folderName_path.insert(folderName, config.Path);
}

void NavigationTree::handleInstanceDeleted(const QString &instanceName)
{
    instanceName_ip.remove(instanceName);
    instanceName_path.remove(instanceName);
    instanceName_date.remove(instanceName);
}

void NavigationTree::handleFolderDeleted(const QString &folderName)
{
    folderName_path.remove(folderName);
}

void NavigationTree::addFolder(const QString &folderName)
{
    CustomTreeWidgetItem *folderItem = new CustomTreeWidgetItem(currentNode, CustomTreeWidgetItem::FolderType);
    folderItem->setText(0, folderName);
    folderItem->setIcon(0, QIcon(folder));

    this->sortItems(0, Qt::AscendingOrder);

    Folder_Node_Config config;
    config.Path = getNodePath(folderItem);
    config.Type = "folder";

    emit folderCreated(config, folderName);
}

void NavigationTree::addInstance(const QString &instanceName, const QString &ipAddress)
{
    CustomTreeWidgetItem *instanceItem = new CustomTreeWidgetItem(currentNode, CustomTreeWidgetItem::NodeType);
    instanceItem->setText(0, instanceName);
    instanceItem->setIcon(0, QIcon(instance));

    this->sortItems(0, Qt::AscendingOrder);

    QDateTime currentDateTime = QDateTime::currentDateTime();

    Instance_Node_Config config;
    config.IP = ipAddress;
    config.Path = getNodePath(instanceItem);
    config.Date = currentDateTime.toString();
    config.Type = "instance";

    emit instanceCreated(config, instanceName);
}

void NavigationTree::deleteCurrentNode()
{
    QTreeWidgetItem *parentItem = currentNode->parent();
    if (parentItem)
    {
        int relativeIndex = parentItem->indexOfChild(currentNode);
        parentItem->takeChild(relativeIndex);

        QString currentNodeName = currentNode->text(0);
        if (currentNode->type() == CustomTreeWidgetItem::NodeType)
        {

            emit instanceDeleted(currentNodeName);
        }
        else
        {
            emit folderDeleted(currentNodeName);
        }

        delete currentNode;
    }
}

void NavigationTree::renameNode()
{
    bool ok = false;
    QString newName;

    // setCurrentLevelSet(currentNode);
    setTreeSet(rootNode.data());

    while (!ok)
    {
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

        QString previousNodeName = currentNode->text(0);
        currentNode->setText(0, newName);
        QString currentNodeName = newName;

        if (currentNode->type() == CustomTreeWidgetItem::NodeType)
        {
            emit instanceNameChanged(previousNodeName, currentNodeName);
        }
        else
        {
            emit folderNameChanged(previousNodeName, currentNodeName);
        }
    }
}

void NavigationTree::writeStringToFile(const QString &str, const QString &filePath)
{
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);
        out << str;
        file.close();
        qDebug() << "String written to file successfully.";
    }
    else
    {
        qDebug() << "Failed to open the file for writing.";
    }
}

void NavigationTree::readStringFromFile(QString &result, const QString &filePath)
{
    QFile file(filePath);

    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream in(&file);
        in >> result;
        file.close();
        qDebug() << "String read from file: " << result;
    }
    else
    {
        qDebug() << "Failed to open the file for reading.";
    }
}

void NavigationTree::createConfigFile()
{
    qDebug()<<folderConfigFile;
    qDebug()<<instanceConfigFile;
    if (!isDirExist(configFilePath))
    {
        QDir dir;
        if (dir.mkpath(configFilePath))
        {
            qDebug() << "Config file directory created successfully.";
        }
        else
        {
            qDebug() << "Failed to create the config file directory.";
        }
    }

    if (!isFileExist(folderConfigFile))
    {
        QFile file(folderConfigFile);
        if (file.open(QIODevice::ReadWrite))
        {
            file.close();
            qDebug() << "Create folder config file successfully.";
        }
        else
        {
            qDebug() << "Failed to create folder config file.";
        }
    }

    if (!isFileExist(instanceConfigFile))
    {
        QFile file(instanceConfigFile);
        if (file.open(QIODevice::ReadWrite))
        {
            file.close();
            qDebug() << "Create instance config file successfully.";
        }
        else
        {
            qDebug() << "Failed to create instance config file.";
        }
    }
}

void NavigationTree::serializeFolder(Folder_Node_Config *folderConfig)
{
    if (folderConfig == nullptr)
    {
        return;
    }

    QString _type(QString("[Type : %1] ").arg(folderConfig->Type));
    QString _path(QString("[Path : %1] ").arg(folderConfig->Path));

    QString dataStream = _type + _path;

    writeStringToFile(dataStream, folderConfigFile);
}

void NavigationTree::SerializeInstance(Instance_Node_Config *instanceConfig)
{
    if (instanceConfig == nullptr)
    {
        return;
    }

    QString _type(QString("[Type : %1] ").arg(instanceConfig->Type));
    QString _path(QString("[Path : %1] ").arg(instanceConfig->Path));
    QString _IP  (QString("[IP : %1] ").arg(instanceConfig->IP));
    QString _Date(QString("[Date : %1] ").arg(instanceConfig->Date));


    QString dataStream = _type + _path + _IP + _Date;

    writeStringToFile(dataStream, instanceConfigFile);
}


