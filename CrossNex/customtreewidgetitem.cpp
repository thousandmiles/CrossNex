#include "customtreewidgetitem.h"

CustomTreeWidgetItem::CustomTreeWidgetItem(QTreeWidgetItem *parent, int type): QTreeWidgetItem(parent, type)
{
    setFlags(flags() | Qt::ItemIsEditable);
}


void CustomTreeWidgetItem::setItemType(ItemType type)
{
    setData(0, Qt::UserRole, type);
}

void CustomTreeWidgetItem::addNode() {
    if (getItemType() == FolderType) {
        CustomTreeWidgetItem *newNode = new CustomTreeWidgetItem(this, NodeType);
        newNode->setText(0, "新建节点");
    }
}
