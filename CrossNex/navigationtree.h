#ifndef NAVIGATIONTREE_H
#define NAVIGATIONTREE_H

#include <QObject>
#include <QTreeWidget>
#include <QWidget>
#include <QMenu>
#include <QMouseEvent>

typedef struct FOLDER_NODE_CONFIG
{
    QString Type; // "folder"
    QString Path; // "/新建/F1/F2/"
}Folder_Node_Config;

typedef struct INSTANCE_NODE_CONFIG
{
    QString Type; // "instance"
    QString Path; // "/新建/F1/F2/aaa"
    QString IP;
    QString Date;
}Instance_Node_Config;


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
    bool isFileExist(const QString& filePath);
    bool isDirExist(const QString& dirPath);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    QString getNodePath(QTreeWidgetItem *node);


private slots:
    void showContextMenu(const QPoint &pos);
    void createFolder();
    void createInstance();
    void handleNodeClicked(QTreeWidgetItem *item, int column);
    void handleInstanceNameChanged(const QString& previous, const QString& current);
    void handleInstanceCreated(const Instance_Node_Config& config, const QString& instanceName);
    void handleFolderNameChanged(const QString& previous, const QString& current);
    void handleFolderCreated(const Folder_Node_Config& config, const QString& folderName);
    void handleInstanceDeleted(const QString& instanceName);
    void handleFolderDeleted(const QString& folderName);


private:
    QScopedPointer<QTreeWidgetItem> rootNode;
    QTreeWidgetItem *currentNode;
    QMenu contextMenu;
    QSet<QString> instanceSet;          // 所有实例名
    QSet<QString> folderSet;            // 所有文件夹名
    QSet<QString> currentLevelInstanceSet;
    QSet<QString> currentLevelFolderSet;
    QSet<QString> nextLevelInstanceSet;
    QSet<QString> nextLevelFolderSet;

    QList<QTreeWidgetItem *>currentLevelInstanceList;
    QList<QTreeWidgetItem *>currentLevelFolderList;

    QString configFilePath;
    QString instanceConfigFile;
    QString folderConfigFile;

    QHash<QString, QString> instanceName_ip;    // 维护所有的实例名及对应IP地址，【新建/重命名/删除】实例时要更新它
    QHash<QString, QString> instanceName_path;
    QHash<QString, QString> instanceName_date;

    QHash<QString, QString> folderName_path;

    bool isParentFolderNameUsed;
    bool isTreeLoaded;

private:
    void addFolder(const QString &folderName);
    void addInstance(const QString &instanceName, const QString &ipAddress, const QString &createTime);
    void deleteCurrentNode();
    void renameNode();
    void writeStringToFile(const QString& str, const QString &filePath);
    void createConfigFile();
    void serializeFolder(Folder_Node_Config* folderConfig);
    void serializeInstance(Instance_Node_Config* instanceConfig);
    void writeConfigToFile();
    void removeConfigFile();
    void reloadTreeFromConfig();
    void reloadFolderConfig(const QString &folderCfg);
    void reloadInstanceConfig(const QString &instanceCfg);
    void createFolderFromConfig(const QStringList &folderList);
    void createInstanceFromConfig(const QString &instance,
                                  const QString &parent,
                                  const QString &ip,
                                  const QString &date);


signals:
    void nodeClicked(const QString& nodeName, const QString& ipAddress);
    void instanceNameChanged(const QString& previous, const QString& current);
    void instanceCreated(const Instance_Node_Config& config, const QString& instanceName);
    void instanceDeleted(const QString& instanceName);

    void folderNameChanged(const QString& previous, const QString& current);
    void folderCreated(const Folder_Node_Config& config, const QString& folderName);
    void folderDeleted(const QString& instanceName);

    void writeConfig();
};

#endif // NAVIGATIONTREE_H
