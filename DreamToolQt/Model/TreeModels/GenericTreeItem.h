/*
 * GenericTreeItem.h
 *
 * Created: 15 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */
#ifndef GENERIC_TREE_TREEITEM_H
#define GENERIC_TREE_TREEITEM_H

#include <QList>
#include <QVariant>

enum GenericTreeItemType
{
    PROJECT,
    ASSET_DEFINITION,
    SCENE,
    SCENE_OBJECT,
    TREE_NODE
};

class GenericTreeItem
{
public:
    explicit GenericTreeItem(const QList<QVariant> &data, GenericTreeItemType type, GenericTreeItem *parentItem = 0);
    ~GenericTreeItem();

    void appendChild(GenericTreeItem *child);
    GenericTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    GenericTreeItem *parentItem();
    GenericTreeItemType getItemType();

private:
    QList<GenericTreeItem*> mChildItems;
    QList<QVariant> mItemData;
    GenericTreeItem *mParentItem;
    GenericTreeItemType mType;
    std::string mUuid;
};

#endif // GENERIC_TREE_TREEITEM_H
