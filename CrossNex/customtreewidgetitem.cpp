#include "customtreewidgetitem.h"

CustomTreeWidgetItem::CustomTreeWidgetItem(QTreeWidgetItem *parent, int type): QTreeWidgetItem(parent, type)
{
    setFlags(flags() | Qt::ItemIsEditable);
}


void CustomTreeWidgetItem::setItemType(ItemType type)
{
    setData(0, Qt::UserRole, type);
}
