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

    CustomTreeWidgetItem(QTreeWidget *parent, int type = NodeType);

    void setItemType(ItemType type);

    ItemType getItemType() const
    {
        return static_cast<ItemType>(data(0, Qt::UserRole).toInt());
    }
};

#endif // CUSTOMTREEWIDGETITEM_H
