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

    bool operator<(const QTreeWidgetItem &other) const override {
        // 按照type排序
        if (type() != other.type())
            return type() < other.type();

        // 在相同type的情况下按照名字排序
        return text(0) < other.text(0);
    }
public:
    QString ipAddress;

public slots:
    void addNode();

    void myItemClicked(CustomTreeWidgetItem *item, int column);
};

#endif // CUSTOMTREEWIDGETITEM_H
