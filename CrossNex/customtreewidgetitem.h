#ifndef CUSTOMTREEWIDGETITEM_H
#define CUSTOMTREEWIDGETITEM_H

#include <QObject>
#include <QMenu>
#include <QTreeWidgetItem>

class CustomTreeWidgetItem : public QTreeWidgetItem
{
public:
    enum ItemType {
        FolderType = UserType + 1,
        NodeType
    };

    CustomTreeWidgetItem(QTreeWidgetItem *parent, int type = NodeType);

    void setItemType(ItemType type);

    ItemType getItemType() const
    {
        return static_cast<ItemType>(data(0, Qt::UserRole).toInt());
    }

public slots:
    void addNode() {
        if (getItemType() == FolderType) {
            CustomTreeWidgetItem *newNode = new CustomTreeWidgetItem(this, NodeType);
            newNode->setText(0, "新建节点");
        }
    }
};

#endif // CUSTOMTREEWIDGETITEM_H
